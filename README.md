# A Particle Filter Tracker for synthetic sequence generation
A particle Filter tracker that assumes 1st order dynamics motion model and uses Color Histograms as Observation Model.

Dependencies
======================
- OpenCV3.2.0
- GSL
Installation Instruction (on Ubuntu16.04)

OpenCV with CUDA acceleration
```
sudo apt-get install -y liblapacke-dev checkinstall
git clone https://github.com/Jim61C/opencv_install_sh.git
cd opencv_install_sh
chmod 755 install_opencv.sh
./install_opencv.sh
cd ..
```

GNU Scientific Library (GSL)
```
sudo apt install -y libgsl2 libgsl-dev
```


Compilation
======================
```
git clone https://github.com/Jim61C/volvo_synthetic_sequence.git
cd volvo_synthetic_sequence
mkdir build
cd build
cmake ..
make
```

Run Instructions
======================
The dataformat that the Particle Filter Tracker currently supports is in [OTB](http://cvlab.hanyang.ac.kr/tracker_benchmark/datasets.html). Once downloaded, one can run a particular sequence as the following:
```
./PFTracker [options] <data_source> <input_dir> <groundtruth_rect_path> 
```
For instance,
```
./PFTracker OTB ../data/Skating2/ ../data/Skating2/groundtruth_rect.txt
```
To save the tracked results, pass in the optional output folder argument:
```
./PFTracker -o ../output/Skating2.mp4 OTB ../data/Skating2/ ../data/Skating2/groundtruth_rect.txt
```

Example Results
======================
[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/AWHD204KHFo/0.jpg)](https://www.youtube.com/watch?v=AWHD204KHFo)
[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/ax4ERbXg_B0/0.jpg)](https://www.youtube.com/watch?v=ax4ERbXg_B0)
[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/fwd87FfnZrM/0.jpg)](https://www.youtube.com/watch?v=fwd87FfnZrM)
[![IMAGE ALT TEXT HERE](https://img.youtube.com/vi/yoEqUmQWG0M/0.jpg)](https://www.youtube.com/watch?v=yoEqUmQWG0M)
