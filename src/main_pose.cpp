#include <iostream>
#include <string>
#include "Pose/PoseRepre.h"
#include "ParticleFilter/BoundingBox.h"
#include <assert.h>  
#include <math.h>
using namespace std;


void printFeature(vector<double> & feature) {
    for (int i =0;i< feature.size();i++) {
        cout << feature[i] << " ";
    }
    cout << endl;
}

void populateTestSinglePose() {
    cout << "in populateTestSinglePose, test loading from txt with a single person's pose and generate features..." << endl;
    string pose_input_path = "/home/jimxing/proj/CPM/Realtime_Multi-Person_Pose_Estimation/testing/sample_image/Skater2_0001_pose.txt";
    string input_frame_path = "/home/jimxing/proj/CPM/Realtime_Multi-Person_Pose_Estimation/testing/sample_image/Skater2_0001.jpg";
    Mat frame = imread(input_frame_path, CV_LOAD_IMAGE_UNCHANGED);
    imshow("frame", frame);
    waitKey(0);
    // test loading pose
    vector<Joint> joints_loaded = PoseRepre::loadOnePoseFromFile(pose_input_path);
    PoseRepre::printJoints(joints_loaded);
    Mat frame_with_pose = frame.clone();
    // test drawing
    PoseRepre::drawPoseOnFrame(frame_with_pose, joints_loaded);
    imshow("frame", frame_with_pose);
    waitKey(0);

    Joint & neck_pos_2d = joints_loaded[1];
    
    // test KinematricTree feaure representation
    KinematicTreeNode *root = PoseRepre::constructKinematicTreeAuto(joints_loaded);
    // test get feature
    vector<double> feature = PoseRepre::getKinematicFeatureRepreGivenNode(root);
    cout << "feature.size():" << feature.size() << endl;
    cout << "original feature: ";
    printFeature(feature);

    // test recover back
    vector<Joint> recovered_joints = PoseRepre::recoverJointsFromKinematicTree(root, neck_pos_2d);
    // PoseRepre::printJoints(recovered_joints);
    assert(recovered_joints.size() == joints_loaded.size());
    double epsilon = pow(10, -3);
    for (int i = 0;i<recovered_joints.size();i++) {
        assert(recovered_joints[i].is_2d == joints_loaded[i].is_2d);
        assert(recovered_joints[i].name.compare(joints_loaded[i].name) == 0);
        assert(abs(recovered_joints[i].x - joints_loaded[i].x) < epsilon);
        assert(abs(recovered_joints[i].y - joints_loaded[i].y) < epsilon);
    }

    // test boundingBox
    BoundingBox box = PoseRepre::getBoundingBoxFromPose(joints_loaded, frame_with_pose);
    box.DrawBoundingBox(frame_with_pose);
    imshow("frame", frame_with_pose);
    waitKey(0);

    // test deepcopy + perturb
    KinematicTreeNode *root_copy = new KinematicTreeNode(root);
    PoseRepre::deformPoseRandom(root_copy);
    vector<double> deformed_feature = PoseRepre::getKinematicFeatureRepreGivenNode(root_copy);
    cout << "deformed feature: ";
    printFeature(deformed_feature);

    vector<Joint> deformed_joints = PoseRepre::recoverJointsFromKinematicTree(root_copy, neck_pos_2d);
    PoseRepre::printJoints(deformed_joints);

    vector<Joint> original_joints = PoseRepre::recoverJointsFromKinematicTree(root, neck_pos_2d);
    
    Mat frame_with_original_pose = frame.clone();
    // test drawing original pose
    PoseRepre::drawPoseOnFrame(frame_with_original_pose, original_joints);
    imshow("original pose", frame_with_original_pose);
    waitKey(0);

    Mat frame_with_deformed_pose = frame.clone();
    // test drawing deformed pose
    PoseRepre::drawPoseOnFrame(frame_with_deformed_pose, deformed_joints);
    imshow("deformed pose", frame_with_deformed_pose);
    waitKey(0);
    


    delete root; // must be there !!!
    delete root_copy;
}

void populateTestMultiPose() {
    cout << "in populateTestMultiPose: Test loading frame with multiple people's poses." << endl;
    string pose_input_path = "/home/jimxing/proj/CPM/Realtime_Multi-Person_Pose_Estimation/testing/sample_image/ski_all_poses.txt";
    string input_frame_path = "/home/jimxing/proj/CPM/Realtime_Multi-Person_Pose_Estimation/testing/sample_image/ski.jpg";
    Mat frame = imread(input_frame_path, CV_LOAD_IMAGE_UNCHANGED);
    imshow("frame", frame);
    waitKey(0);

    // test loading mutliple people's joint inside
    vector<vector<Joint> > multi_joints_loaded = PoseRepre::loadPosesFromFile(pose_input_path);
    Mat frame_with_pose = frame.clone();
    for (int i =0;i< multi_joints_loaded.size();i++) {
        vector <Joint> & this_loaded_joints = multi_joints_loaded[i];
        PoseRepre::drawPoseOnFrame(frame_with_pose, this_loaded_joints);
    }

    // visualise
    imshow("frame_with_pose", frame_with_pose);
    waitKey(0);

    // test pose given BoundingBox b1
    BoundingBox b1(127, 243, 113, 168);
    vector<Joint> joints_b1  = PoseRepre::findPoseInBoundingBox(multi_joints_loaded, b1);
    Mat frame_with_one_pose = frame.clone();
    PoseRepre::drawPoseOnFrame(frame_with_one_pose, joints_b1);
    imshow("frame_with_one_pose", frame_with_one_pose);
    waitKey(0);

    // test pose given BoundingBox b2
    BoundingBox b2(545, 19, 126, 383);
    vector<Joint> joints_b2  = PoseRepre::findPoseInBoundingBox(multi_joints_loaded, b2);
    frame_with_one_pose = frame.clone();
    PoseRepre::drawPoseOnFrame(frame_with_one_pose, joints_b2);
    imshow("frame_with_one_pose", frame_with_one_pose);
    waitKey(0);

    // test pose given BoundingBox b3
    BoundingBox b3(545, 19, 126, 150);
    vector<Joint> joints_b3  = PoseRepre::findPoseInBoundingBox(multi_joints_loaded, b3);
    frame_with_one_pose = frame.clone();
    PoseRepre::drawPoseOnFrame(frame_with_one_pose, joints_b3);
    imshow("frame_with_one_pose", frame_with_one_pose);
    waitKey(0);
}

void populateTestPartialPose() {
    cout << "in populateTestPartialPose: Test loading frame with partial people's poses." << endl;
    string pose_input_path = "/home/jimxing/proj/CPM/Realtime_Multi-Person_Pose_Estimation/testing/sample_image/upper2_poses.txt";
    string input_frame_path = "/home/jimxing/proj/CPM/Realtime_Multi-Person_Pose_Estimation/testing/sample_image/upper2.jpg";
    Mat frame = imread(input_frame_path, CV_LOAD_IMAGE_UNCHANGED);
    imshow("frame", frame);
    waitKey(0);


    // test loading mutliple people's joint inside
    vector<vector<Joint> > multi_joints_loaded = PoseRepre::loadPosesFromFile(pose_input_path);
    Mat frame_with_pose = frame.clone();
    for (int i =0;i< multi_joints_loaded.size();i++) {
        vector <Joint> & this_loaded_joints = multi_joints_loaded[i];
        PoseRepre::drawPoseOnFrame(frame_with_pose, this_loaded_joints);
    }

    // visualise
    imshow("frame_with_pose", frame_with_pose);
    waitKey(0);

    // Test partial poses to kinematic tree feature representation
    for (int i =0;i< multi_joints_loaded.size();i++) {
        vector <Joint> & this_loaded_joints = multi_joints_loaded[i];
        Joint & neck_pos_2d = this_loaded_joints[1];
        KinematicTreeNode *root = PoseRepre::constructKinematicTreeAuto(this_loaded_joints);

        // test get feature
        vector<double> feature = PoseRepre::getKinematicFeatureRepreGivenNode(root);
        cout << "feature.size():" << feature.size() << endl;
        cout << "original feature: ";
        printFeature(feature);

        // test recover back
        vector<Joint> recovered_joints = PoseRepre::recoverJointsFromKinematicTree(root, neck_pos_2d);
        
        frame_with_pose = frame.clone();
        // test drawing
        PoseRepre::drawPoseOnFrame(frame_with_pose, recovered_joints);
        imshow("recovered joints:", frame_with_pose);
        waitKey(0);
        delete root;
    }

}

void populateSaveNYCPosesKinematicTreeFeature() {
    cout << "in populateSaveNYCPosesKinematicTreeFeature:..." << endl;
    string pose_input_path = "/home/jimxing/proj/CPM/Realtime_Multi-Person_Pose_Estimation/testing/sample_image/nyc_pose.txt";
    
    int W = 1024;
    int H = 1024;
    Mat canvas(H, W, CV_8UC3, Scalar(0,0,0));
    imshow("canvas", canvas);
    waitKey(0);

    // test loading mutliple people's joint inside
    vector<vector<Joint> > multi_joints_loaded = PoseRepre::loadPosesFromFile(pose_input_path);
    cout << "number of poses loaded:" << multi_joints_loaded.size() << endl;
    // visualise 10 of them
    Mat canvas_with_pose = canvas.clone();
    for (int i =0;i< 10;i++) {
        vector <Joint> & this_loaded_joints = multi_joints_loaded[i];
        PoseRepre::drawPoseOnFrame(canvas_with_pose, this_loaded_joints);
        imshow("canvas_with_pose", canvas_with_pose);
        waitKey(0);
    }

    vector<vector<double> > pose_features;
    for (int i = 0; i< multi_joints_loaded.size(); i ++ ) {
        vector<Joint> & this_loaded_joints = multi_joints_loaded[i];
        Joint & neck_pos_2d = this_loaded_joints[1];
        KinematicTreeNode *root = PoseRepre::constructKinematicTreeAuto(this_loaded_joints);
        vector<double> this_feature = PoseRepre::getKinematicFeatureRepreGivenNode(root);
        printFeature(this_feature);
        delete root;
    }
}

void visualiseNYCCentroids() {
    cout << "in visualiseNYCCentroids:..." << endl;
    string pose_input_path = "/home/jimxing/proj/CPM/Realtime_Multi-Person_Pose_Estimation/testing/sample_image/nyc_pose_centroids_2d.txt";

    int W = 1024;
    int H = 1024;
    Mat canvas(H, W, CV_8UC3, Scalar(0,0,0));
    imshow("canvas", canvas);
    waitKey(0);

    vector<vector<Joint> > multi_joints_loaded = PoseRepre::loadPosesFromFile(pose_input_path);
    cout << multi_joints_loaded.size() << endl;

    Mat canvas_with_pose = canvas.clone();
    for (int i =0;i< 10;i++) {
        vector <Joint> & this_loaded_joints = multi_joints_loaded[i];
        cout << i << " joints loaded " << endl; 
        PoseRepre::printJoints(this_loaded_joints);
        PoseRepre::drawPoseOnFrame(canvas_with_pose, this_loaded_joints);
        imshow("canvas", canvas_with_pose);
        waitKey(0);
    }
}

void visualiseNYCCentroidsKinematicFeature() {
    cout << "in visualiseNYCCentroidsKinematicFeature..." << endl;
    string kinematic_features_input = "/home/jimxing/proj/CPM/Realtime_Multi-Person_Pose_Estimation/testing/sample_image/nyc_pose_centroids_kinematic.txt";
    
    int W = 1024;
    int H = 1024;
    Mat canvas(H, W, CV_8UC3, Scalar(0,0,0));
    imshow("canvas", canvas);
    waitKey(0);

    vector<vector <double> > loaded_features = PoseRepre::loadFeaturesFromFile(kinematic_features_input);
    Mat canvas_with_pose = canvas.clone();
    for (int i =0;i< loaded_features.size();i++) {
        Joint neck_pos_2d(500, 500, "neck");
        vector<double> & this_feature = loaded_features[i];
        printFeature(this_feature);
        KinematicTreeNode *root = PoseRepre::constructKinematicTreeFromFeatures(this_feature);
        
        // validate that feature is the same
        vector<double> validate_feature = PoseRepre::getKinematicFeatureRepreGivenNode(root);
        printFeature(validate_feature);

        // test recover back
        vector<Joint> recovered_joints = PoseRepre::recoverJointsFromKinematicTree(root, neck_pos_2d);
        
        // test drawing
        canvas_with_pose = canvas.clone();
        PoseRepre::drawPoseOnFrame(canvas_with_pose, recovered_joints);
        imshow("recovered joints:", canvas_with_pose);
        waitKey(0);
        delete root;
    }
}

int main(int argc, char **argv) {
    // populateTestSinglePose();
    // populateTestMultiPose();
    populateTestPartialPose();
    // visualiseNYCCentroids();
    // populateSaveNYCPosesKinematicTreeFeature();
    // visualiseNYCCentroidsKinematicFeature();
    return 0;
}