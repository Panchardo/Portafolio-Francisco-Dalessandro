void setup() {
  system("ifconfig eth0 inet 192.168.100.120 netmask 255.255.255.0 up");
  system("telnetd");
}

void loop() {
}