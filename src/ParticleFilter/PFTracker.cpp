 #include "PFTracker.h"
 
 void PFTracker::start(DataManager & dm) {
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
    this->pf.DrawParticles(frame_0);
    imshow("frame", frame_0);
    waitKey(0);

    // start procedure
    for (int i = 1;i< dm.frames.size(); i++) {
        Mat cur_frame = dm.frames[i];
        
        // before transition
        this->pf.DrawParticles(cur_frame);
        imshow("frame", cur_frame);
        waitKey(0);

        // motion model
        this->pf.transition(cur_frame.size().width, cur_frame.size().height);

        //draw resampled particles
        this->pf.DrawParticles(cur_frame);
        imshow("frame", cur_frame);
        waitKey(0);
        

        // observation model
        this->pf.updateWeights(cur_frame);

        // posterior
        this->pf.updateCurrentROI(cur_frame);

        // resample
        this->pf.resampleParticles();

        //draw resampled particles
        this->pf.DrawParticles(cur_frame);

        // draw estimate
        this->pf.current_roi.roi.Draw(0, 255, 0, cur_frame);

        // draw gt
        BoundingBox cur_gt;
        cur_gt.setBox(dm.boxes[0].colRange(0, 4).rowRange(i,i+1));
        cur_gt.DrawBoundingBox(cur_frame);
        
        // visualise
        imshow("frame", cur_frame);
        waitKey(0);
    }
 }