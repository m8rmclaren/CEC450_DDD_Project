//
// Created by Hayden Roszell on 12/1/22.
//

#include "CompareValues.h"


using namespace std;

void CompareValues::run() {
    IMUValues average = {0, 0, 0, 0, 0, 0, 0};

    IMUValues values;
    mutex->lock();
    for (int i = 0; i < 4; ++i) {
        values = adcvals->back();
        adcvals->pop_back();
        average.accelX >>= 1;
        average.accelY >>= 1;
        average.accelZ >>= 1;
        average.gyroX >>= 1;
        average.gyroY >>= 1;
        average.gyroZ >>= 1;
        average.temp /= 2;
        average.accelX += values.accelX;
        average.accelY += values.accelY;
        average.accelZ += values.accelZ;
        average.gyroX += values.gyroX;
        average.gyroY += values.gyroY;
        average.gyroZ += values.gyroZ;
        average.temp += values.temp;
    }
    adcvals->clear();
    mutex->unlock();
    average.accelX >>= 1;
    average.accelY >>= 1;
    average.accelZ >>= 1;
    average.gyroX >>= 1;
    average.gyroY >>= 1;
    average.gyroZ >>= 1;
    average.temp /= 2;


    // log change from last
//    cout << "accelX: " << average.accelX - last.accelX << endl;
//    cout << "accelY: " << average.accelY - last.accelY << endl;
//    cout << "accelZ: " << average.accelZ - last.accelZ << endl;
//    cout << "gyroX: " << average.gyroX - last.gyroX << endl;
//    cout << "gyroY: " << average.gyroY - last.gyroY << endl;
//    cout << "gyroZ: " << average.gyroZ - last.gyroZ << endl;
//    cout << "temp: " << average.temp - last.temp << endl;
    // log in change from last in csv form
//    cout << average.accelX - last.accelX << "," << average.accelY - last.accelY << "," << average.accelZ - last.accelZ << ","
//         << average.gyroX - last.gyroX << "," << average.gyroY - last.gyroY << "," << average.gyroZ - last.gyroZ << ","
//         << average.temp - last.temp << endl;

    if (abs(average.accelX - last.accelX) > RED ||
        abs(average.accelY - last.accelY) > RED) {

        red_pin.set_value(1);
        yellow_pin.set_value(0);
        green_pin.set_value(0);
        std::this_thread::sleep_for(1s);
    }

    else if (abs(average.accelX - last.accelX) > YELLOW ||
        abs(average.accelY - last.accelY) > YELLOW) {

        red_pin.set_value(0);
        yellow_pin.set_value(1);
        green_pin.set_value(0);
    }

    else {

        red_pin.set_value(0);
        yellow_pin.set_value(0);
        green_pin.set_value(1);
    }


    last = average;
}