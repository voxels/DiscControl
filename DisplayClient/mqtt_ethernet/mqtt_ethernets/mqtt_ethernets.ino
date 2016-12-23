

void setup()
{
  lightsSetup();
  ethernetsSetup();  
}

void loop()
{
  if ( loopEthernet() ) {
    loopLights();
  }
}

