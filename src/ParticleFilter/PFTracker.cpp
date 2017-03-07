 #include "PFTracker.h"
 
 void PFTracker::start(DataManager & dm) {
    cout << dm.boxes[0].rows << ", " << dm.boxes[0].cols << endl;
    this->initial_box.setBox(dm.boxes[0].colRange(0, 4).rowRange(0,1));

    // prior, initialise particles
    Mat frame_0 = dm.frames[0];
    this->pf.template_roi.roi.setAsBox(this->initial_box);
    this->pf.current_roi.roi.setAsBox(this->initial_box);
    this->pf.initParticles(frame_0, this->initial_box);

    // visualise the first frame with initial box
    this->initial_box.DrawBoundingBox(frame_0);
    this->pf.DrawParticles(frame_0);
    imshow("frame", frame_0);
    waitKey(0);

    // start procedure
    for (int i = 1;i< dm.frames.size(); i++) {
        Mat cur_frame = dm.frames[i];

        //draw particles
        this->pf.DrawParticles(cur_frame);

        // draw estimate
        this->pf.current_roi.roi.Draw(0, 255, 0, cur_frame);

        // add gt
        BoundingBox cur_gt;
        cur_gt.setBox(dm.boxes[0].colRange(0, 4).rowRange(i,i+1));
        cur_gt.DrawBoundingBox(cur_frame);
        
        // visualise
        imshow("frame", cur_frame);
        waitKey(0);
    }
 }