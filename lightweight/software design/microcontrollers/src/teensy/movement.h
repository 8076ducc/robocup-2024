#define ballBearing, ballDist
void trackBall() {
    float ballOffset, OffsetMult;
    if (ballBearing <= 180)
    ballOffset = fmin(ballBearing * 1.0, 90);
    else 
    ballOffset = fmax((ballBearing - 360) * 1.0, -90);
//tune multiplier 1.0

float factor = 1 - ballDist/90; //factor represents how 'close' the ball is
//adjust max ballDist constant 90
OffsetMult = fmin(0.1 * exp(4.0 * factor), 1); 
// tune constants 0.1 & 4.0

robotBearing = ballBearing + ballOffset * OffsetMult;
}

//#define goalBearing
void trackGoal() {
    float goalOffset;
    if (goalBearing <= 180)
    goalOffset = fmin(goalBearing, 90);
    else
    goalOffset = fmax((goalBearing - 360), -90);

robotBearing = goalBearing + goalOffset * 1; 
//tune constant goalOffset Multiplier 1  
}


