rm -f Train.exe 
#g++ -I. `root-config --libs --cflags` -lRooFitCore -lRooFit -lRooStats -lTMVA -lXMLIO -lTreePlayer -o  Train.exe nominal_train.cpp
g++ `root-config --libs --cflags` -lTMVA -lXMLIO -o  Train.exe nominal_train.cpp
./Train.exe 