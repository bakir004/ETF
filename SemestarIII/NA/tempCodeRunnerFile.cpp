atrix B{{0,3},{4,6},{3,1}};
    QRDecomposer qr2(B);
    std::cout<<B.EqualTo(qr2.GetQ() * qr2.GetR());;