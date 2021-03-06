 #include "PFTracker.h"
//  #define DRAW_PARTICLES
 #define DRAW_GT
 
// #define DRAW_PARTICLES
#define DRAW_GT
 
 void PFTracker::start(DataManager & dm, VideoWriter & writer) {
    cout << "single tracking boxes.shape:" << dm.boxes[0].rows << ", " << dm.boxes[0].cols << endl;
    this->initial_box.setBox(dm.boxes[0].colRange(0, 4).rowRange(0,1));

    Mat frame_0 = dm.frames[0];
    // set template_roi, current_roi, compute template features
    this->pf.current_roi.roi.setAsBox(this->initial_box);
    MatND hist = ParticleFilter::computeColorHistogram(this->pf.current_roi.roi, frame_0);
    this->pf.current_roi.color_feature = hist;
    this->pf.template_roi.roi.setAsBox(this->initial_box);
    this->pf.template_roi.color_feature = hist;
    // prior, initialise particles
    this->pf.initParticles(frame_0, this->initial_box);

    // visualise the first frame with initial box
    this->initial_box.DrawBoundingBox(frame_0);
#ifdef DRAW_PARTICLES
    this->pf.DrawParticles(frame_0);
    imshow("frame", frame_0);
#endif
    waitKey(WAIT_KEY_NUM_MILLISECONDS);

    // write first frame if writer is open
    if (writer.isOpened()) {
        writer << frame_0;
    }

    // start procedure
    for (int i = 1;i< dm.frames.size(); i++) {
        Mat cur_frame = dm.frames[i];
        Mat cur_frame_to_display = cur_frame.clone();
        
        // before transition
#ifdef DRAW_PARTICLES
        cout << "before particle transition" << endl;
        this->pf.DrawParticles(cur_frame_to_display);
        imshow("frame", cur_frame_to_display);
        waitKey(WAIT_KEY_NUM_MILLISECONDS);
#endif 


        // motion model
        this->pf.transition(cur_frame.size().width, cur_frame.size().height);

        //draw moved particles
#ifdef DRAW_PARTICLES
        cout << "draw moved particles" << endl;
        cur_frame_to_display = cur_frame.clone();
        this->pf.DrawParticles(cur_frame_to_display);
        imshow("frame", cur_frame_to_display);
        waitKey(WAIT_KEY_NUM_MILLISECONDS);
#endif

        // observation model
        this->pf.updateWeights(cur_frame);


        //draw particles with their weigts indicating colors
#ifdef DRAW_PARTICLES
        cout << "after weights, draw particles with colors indicating weights" << endl;
        cur_frame_to_display = cur_frame.clone();
        this->pf.DrawParticles(cur_frame_to_display);
        imshow("frame", cur_frame_to_display);
        waitKey(WAIT_KEY_NUM_MILLISECONDS);
#endif 
       

        // posterior
        this->pf.updateCurrentROI(cur_frame);

        // resample
        this->pf.resampleParticles();

        //draw resampled particles, 
#ifdef DRAW_PARTICLES
        cout << "after resampling of particles " << endl;
        cur_frame_to_display = cur_frame.clone();
        this->pf.DrawParticles(cur_frame_to_display);
#endif

        // draw estimate
        this->pf.current_roi.roi.Draw(0, 255, 0, cur_frame_to_display);

#ifdef DRAW_GT
        // draw gt
        BoundingBox cur_gt;
        cur_gt.setBox(dm.boxes[0].colRange(0, 4).rowRange(i,i+1));
        cur_gt.DrawBoundingBox(cur_frame_to_display);
#endif
        
        // visualise
        imshow("frame", cur_frame_to_display);
        waitKey(WAIT_KEY_NUM_MILLISECONDS);

        // write cur frame if writer is open
        if (writer.isOpened()) {
            writer << cur_frame_to_display;
        }

    }
 }
