# GenICam-GenTL-Webcam

Proof-of-concept GenTL producer driver based on GeniCAM GenTL standard:

http://www.emva.org/standards-technology/genicam/

Code partially based on Avaris:

https://github.com/AravisProject/aravis

Status:

* GenAPI works.
* GenTL handshake works.
* Simplified buffer implemented using dummy image generator from Aravis.
* Consumer does not receive images. Frames seems to be accepted but they are all empty.
* Nothing to do with webcam yet.