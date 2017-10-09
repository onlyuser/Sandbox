#include <iostream>
#include <math.h>
#include <stdlib.h>

float sigmoid(float x)
{
    return 1 / (1 + static_cast<float>(exp(-x)));
}

float sigmoidPri(float y)
{
    return y * (1 - y);
}

// https://sites.google.com/site/onlyuser/files/bpnet_graph.jpg
//
// layer0          layer1            layer2
//
// [v0]------w00---[out0|err0]------w20---[out2|err2|v2]
//    \            /  |                   /  |
//     \  *--w01--*  w02         *--w21--*  w22
//      \/           /          /           /
//      /\          /          /           /
//     /  *--w10--*/          /           /
//    /           /\         /           /
// [v1]------w11---[out1|err1]          /
//              /     |                /
//        *----*     w12         *----*
//       /           /          /
//      /   *-------*          /
//     /   /                  /
//    /   /                  /
// [b0]--*                [b1]
//
// bias==1                bias==1

int main(int argc, char** argv)
{
    float w[3][3];
    float v[3];
    float out[3];
    float err[3];

    // randomize weights
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            w[i][j] = -1 + 2 * ((float) rand() / RAND_MAX);
        }
    }

    // train
    for(int n = 0; n < 1000; n++) {
        for(int k = 0; k < 4; k++) {
            // fill training set
            switch(k) {
                case 0: v[0] = 0; v[1] = 0; v[2] = 0; break;
                case 1: v[0] = 0; v[1] = 1; v[2] = 1; break;
                case 2: v[0] = 1; v[1] = 0; v[2] = 1; break;
                case 3: v[0] = 1; v[1] = 1; v[2] = 0; break;
            }

            // propagate forward
            out[0] = sigmoid(v[0]   * w[0][0] +
                             v[1]   * w[0][1] +
                             1      * w[0][2]);
            out[1] = sigmoid(v[0]   * w[1][0] +
                             v[1]   * w[1][1] +
                             1      * w[1][2]);
            out[2] = sigmoid(out[0] * w[2][0] +
                             out[1] * w[2][1] +
                             1      * w[2][2]);

            // propagate backward
            err[2] = 1       * sigmoidPri(out[2]) * (v[2] - out[2]);
            err[0] = w[2][0] * sigmoidPri(out[0]) * err[2];
            err[1] = w[2][1] * sigmoidPri(out[1]) * err[2];

            // adjust weights
            w[0][0] += v[0]   * err[0];
            w[0][1] += v[1]   * err[0];
            w[0][2] += 1      * err[0];
            w[1][0] += v[0]   * err[1];
            w[1][1] += v[1]   * err[1];
            w[1][2] += 1      * err[1];
            w[2][0] += out[0] * err[2];
            w[2][1] += out[1] * err[2];
            w[2][2] += 1      * err[2];
        }
    }

    // apply
    for(int k = 0; k < 4; k++) {
        // fill testing set
        switch(k) {
            case 0: v[0] = 0; v[1] = 0; v[2] = -1; break;
            case 1: v[0] = 0; v[1] = 1; v[2] = -1; break;
            case 2: v[0] = 1; v[1] = 0; v[2] = -1; break;
            case 3: v[0] = 1; v[1] = 1; v[2] = -1; break;
        }

        // propagate forward
        out[0] = sigmoid(v[0]   * w[0][0] +
                         v[1]   * w[0][1] +
                         1      * w[0][2]);
        out[1] = sigmoid(v[0]   * w[1][0] +
                         v[1]   * w[1][1] +
                         1      * w[1][2]);
        out[2] = sigmoid(out[0] * w[2][0] +
                         out[1] * w[2][1] +
                         1      * w[2][2]);

        // copy solution
        v[2] = out[2];
        std::cout << v[0] << ", " << v[1] << ", " << v[2] << '\n';
    }
}
