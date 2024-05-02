# Part I, $\eta = 0.1, m = 10, 10 epochs$ 

Epoch: 0,  Total iter: 0,  Mean Loss: 0.000000000000,  Test Acc: 0.097900 
Epoch: 0,  Total iter: 30000,  Mean Loss: 0.290116757279,  Test Acc: 0.938800
Epoch: 1,  Total iter: 60000,  Mean Loss: 0.150084701080,  Test Acc: 0.959000
Epoch: 1,  Total iter: 90000,  Mean Loss: 0.108902740977,  Test Acc: 0.968900
Epoch: 2,  Total iter: 120000,  Mean Loss: 0.087976168406,  Test Acc: 0.969200
Epoch: 2,  Total iter: 150000,  Mean Loss: 0.074308374847,  Test Acc: 0.968600
Epoch: 3,  Total iter: 180000,  Mean Loss: 0.065313537738,  Test Acc: 0.967600
Epoch: 3,  Total iter: 210000,  Mean Loss: 0.058086555357,  Test Acc: 0.973300
Epoch: 4,  Total iter: 240000,  Mean Loss: 0.053965128249,  Test Acc: 0.963000
Epoch: 4,  Total iter: 270000,  Mean Loss: 0.044736088982,  Test Acc: 0.974500
Epoch: 5,  Total iter: 300000,  Mean Loss: 0.045295079852,  Test Acc: 0.972400
Epoch: 5,  Total iter: 330000,  Mean Loss: 0.040072043059,  Test Acc: 0.975200
Epoch: 6,  Total iter: 360000,  Mean Loss: 0.040478170636,  Test Acc: 0.968800
Epoch: 6,  Total iter: 390000,  Mean Loss: 0.032800647996,  Test Acc: 0.973700
Epoch: 7,  Total iter: 420000,  Mean Loss: 0.033630554450,  Test Acc: 0.974600
Epoch: 7,  Total iter: 450000,  Mean Loss: 0.026638022982,  Test Acc: 0.976700
Epoch: 8,  Total iter: 480000,  Mean Loss: 0.027501573541,  Test Acc: 0.977600
Epoch: 8,  Total iter: 510000,  Mean Loss: 0.026845247318,  Test Acc: 0.976800
Epoch: 9,  Total iter: 540000,  Mean Loss: 0.029739986996,  Test Acc: 0.975900
Epoch: 9,  Total iter: 570000,  Mean Loss: 0.025887542784,  Test Acc: 0.978600
Epoch: 10,  Total iter: 600000,  Mean Loss: 0.025989712547,  Test Acc: 0.971400


# Gradient checks 5 random samples per layer

Layer | Error | checks | a_time | n_time 
L3_LO, 0.000000, 5000, 0.005903, 2.913315
L2_L3, 0.000002, 20000, 0.007690, 11.653941
L1_L2, 0.000003, 20000, 0.007316, 11.679194
LI_L1, 0.000003, 20000, 0.006744, 11.665774


Threaded 

Layer | Error | checks | a_time | n_time
L3_LO, 0.000000, 5000, 0.004546, 0.608897
L2_L3, 0.000004, 20000, 0.004608, 3.571736
L1_L2, 0.000003, 20000, 0.004519, 2.168050
LI_L1, 0.000005, 20000, 0.002189, 2.175816

# Adam

Epoch: 0,  Total iter: 0,  Mean Loss: 0.000000000000,  Test Acc: 0.094800, Learning Rate: 0.010000
Epoch: 0,  Total iter: 30000,  Mean Loss: 0.300004780133,  Test Acc: 0.951100, Learning Rate: 0.010000
Epoch: 1,  Total iter: 60000,  Mean Loss: 0.143243544513,  Test Acc: 0.957500, Learning Rate: 0.010000
Epoch: 1,  Total iter: 90000,  Mean Loss: 0.099812394852,  Test Acc: 0.970200, Learning Rate: 0.010000
Epoch: 2,  Total iter: 120000,  Mean Loss: 0.080012710166,  Test Acc: 0.966900, Learning Rate: 0.010000
Epoch: 2,  Total iter: 150000,  Mean Loss: 0.062365945617,  Test Acc: 0.975300, Learning Rate: 0.010000
Epoch: 3,  Total iter: 180000,  Mean Loss: 0.056319730556,  Test Acc: 0.971200, Learning Rate: 0.010000
Epoch: 3,  Total iter: 210000,  Mean Loss: 0.044206042718,  Test Acc: 0.974600, Learning Rate: 0.010000
Epoch: 4,  Total iter: 240000,  Mean Loss: 0.042041993012,  Test Acc: 0.972100, Learning Rate: 0.010000
Epoch: 4,  Total iter: 270000,  Mean Loss: 0.034579315233,  Test Acc: 0.978100, Learning Rate: 0.010000
Epoch: 5,  Total iter: 300000,  Mean Loss: 0.034102259290,  Test Acc: 0.962600, Learning Rate: 0.010000
Epoch: 5,  Total iter: 330000,  Mean Loss: 0.031143193726,  Test Acc: 0.975000, Learning Rate: 0.010000
Epoch: 6,  Total iter: 360000,  Mean Loss: 0.027379519834,  Test Acc: 0.975500, Learning Rate: 0.010000
Epoch: 6,  Total iter: 390000,  Mean Loss: 0.025023619262,  Test Acc: 0.975000, Learning Rate: 0.010000
Epoch: 7,  Total iter: 420000,  Mean Loss: 0.028360136842,  Test Acc: 0.975200, Learning Rate: 0.010000
Epoch: 7,  Total iter: 450000,  Mean Loss: 0.024160063544,  Test Acc: 0.978400, Learning Rate: 0.010000
Epoch: 8,  Total iter: 480000,  Mean Loss: 0.021179979673,  Test Acc: 0.977300, Learning Rate: 0.010000
Epoch: 8,  Total iter: 510000,  Mean Loss: 0.018814615718,  Test Acc: 0.980100, Learning Rate: 0.010000
Epoch: 9,  Total iter: 540000,  Mean Loss: 0.022382536410,  Test Acc: 0.973200, Learning Rate: 0.010000
Epoch: 9,  Total iter: 570000,  Mean Loss: 0.018158887838,  Test Acc: 0.980700, Learning Rate: 0.010000
Epoch: 10,  Total iter: 600000,  Mean Loss: 0.018394954294,  Test Acc: 0.975300, Learning Rate: 0.010000


Optimizing with parameters: 
	Epochs = 10 
	Batch size = 50 
	Total batches = 12000
	Adam Optimizer
	Learning rate = 0.001000
	Beta1 = 0.900000
	Beta2 = 0.999000
	Epsilon = 1.000000e-08

********************************************************************************
Performing training optimisation...
********************************************************************************
Epoch: 0,  Total iter: 0,  Mean Loss: 0.000000000000,  Test Acc: 0.088900, Elapsed: 0.721558
Epoch: 0,  Total iter: 30000,  Mean Loss: 0.323290231550,  Test Acc: 0.946000, Elapsed: 10.097993
Epoch: 1,  Total iter: 60000,  Mean Loss: 0.156759911733,  Test Acc: 0.955700, Elapsed: 11.054906
Epoch: 1,  Total iter: 90000,  Mean Loss: 0.106543437662,  Test Acc: 0.968100, Elapsed: 9.669278
Epoch: 2,  Total iter: 120000,  Mean Loss: 0.089951136519,  Test Acc: 0.965600, Elapsed: 9.629248
Epoch: 2,  Total iter: 150000,  Mean Loss: 0.065342284822,  Test Acc: 0.975000, Elapsed: 10.872970
Epoch: 3,  Total iter: 180000,  Mean Loss: 0.061254117176,  Test Acc: 0.972800, Elapsed: 9.321948
Epoch: 3,  Total iter: 210000,  Mean Loss: 0.045530494188,  Test Acc: 0.979300, Elapsed: 8.929551
Epoch: 4,  Total iter: 240000,  Mean Loss: 0.046017284442,  Test Acc: 0.967600, Elapsed: 11.387055
Epoch: 4,  Total iter: 270000,  Mean Loss: 0.042173727911,  Test Acc: 0.978500, Elapsed: 8.961761
Epoch: 5,  Total iter: 300000,  Mean Loss: 0.039911361633,  Test Acc: 0.976500, Elapsed: 9.129150
Epoch: 5,  Total iter: 330000,  Mean Loss: 0.029716339093,  Test Acc: 0.972600, Elapsed: 11.118518
Epoch: 6,  Total iter: 360000,  Mean Loss: 0.032155174214,  Test Acc: 0.975300, Elapsed: 9.115172
Epoch: 6,  Total iter: 390000,  Mean Loss: 0.026369905132,  Test Acc: 0.979700, Elapsed: 9.166036
Epoch: 7,  Total iter: 420000,  Mean Loss: 0.025032769871,  Test Acc: 0.981100, Elapsed: 11.291423
Epoch: 7,  Total iter: 450000,  Mean Loss: 0.021198104655,  Test Acc: 0.974400, Elapsed: 9.300198
Epoch: 8,  Total iter: 480000,  Mean Loss: 0.024112951188,  Test Acc: 0.968700, Elapsed: 9.359318
Epoch: 8,  Total iter: 510000,  Mean Loss: 0.021839596210,  Test Acc: 0.978200, Elapsed: 11.328666
Epoch: 9,  Total iter: 540000,  Mean Loss: 0.021855863248,  Test Acc: 0.978100, Elapsed: 9.391775
Epoch: 9,  Total iter: 570000,  Mean Loss: 0.020199466049,  Test Acc: 0.981100, Elapsed: 9.359081
Epoch: 10,  Total iter: 600000,  Mean Loss: 0.018375815084,  Test Acc: 0.973700, Elapsed: 10.574139
********************************************************************************
