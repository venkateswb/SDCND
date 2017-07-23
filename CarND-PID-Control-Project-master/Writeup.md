# Steps invloved in tuning PID parameters

1) Firstly the parameters I and D were set to zeros and tuned P parameter, With this car started with good steering angle and slowly after some time car started oscillating left and right. Then  slowly frequency started increasing within short duration of time.
2) Started tuning D paramter with still 'I' at zero. This damped the oscillation when increased. The P and D combination value were slowly adjusted to take all turns with less oscillations. P was increased more to take sharp turns and D was to keep on checking the oscillations.
3) Althogh PD value worked good enough, A small value of I was introduced to correct the error values over long duration of time. A large value of I gave a bad start but over time error was under check. so only a small value was set. 
