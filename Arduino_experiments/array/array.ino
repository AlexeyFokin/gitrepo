
#define FILLARRAY(a,n) a[0]=n, memcpy( ((char*)a)+sizeof(a[0]), a, sizeof(a)-sizeof(a[0]) );

int x[200];

void setup() {
  // put your setup code here, to run once:

  FILLARRAY(x,1345);
}

void loop() {
  // put your main code here, to run repeatedly:

  Serial.println(x[12]);
  delay(1000);
}
