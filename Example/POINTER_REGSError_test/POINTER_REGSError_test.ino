float dhistory[10];
float test;
float getSlope(float history[]) __attribute__((__optimize__("O2")));

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:

  test = getSlope(dhistory);
}

float getSlope(float history[]) {
  float sumx = 0;
  float sumy = 0;
  float sumxy = 0;
  float sumxsq = 0;
  float rate = 0;
  int n = 10;

  for (int i=1; i< 11; i++) {
    sumx = sumx + i;
    sumy = sumy + history[i-1];
    sumy = sumy + history[i-1];
    sumxsq = sumxsq + (i*i);
  } 

  rate = sumy+sumx+sumxsq;
  return rate;
}
