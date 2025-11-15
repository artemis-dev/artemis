# Example for Tracking Simulation 
* 1st rev. by Shinsuke OTA on 2017/06/06 

## How to use
1. Copy the steering files and histgram definition into your working directory.
```
cp -av ./steering/ ${ARTEMIS_WORKDIR}/steering
cp -av ./hist     ${ARTEMIS_WORKDIR}/hist
```

2. Download data files from mail server (as for 2017/06/06, to be changed)
```
rsync -av mail:/home/ota/cat_data/ data
```

If you already have 'data' directory, please change the directory name
for download and the corresponding directory name in steering files.

3. Run with steering file you want.

* steering/cat/simtrack.yaml provides the simulation result only with statistical uncertainty
* steering/cat/simtrack_folded.yaml provides the simulation result with statistical uncertainty and the resolution due to the circuit


## details in preparation

