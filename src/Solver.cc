#include <Eigen/Dense>

Eigen::VectorXd QR_Method(Eigen::MatrixXd A, double const max_iters)
{
    long cnt = 0;
    while (cnt < max_iters) {
        auto QR = A.householderQr();
        auto Q = QR.householderQ();
        A = Q.transpose() * A * Q;
        ++ cnt;
    }
    return A.diagonal();
}