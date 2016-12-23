

void setup()
{
  lightsSetup();
  ethernetsSetup();  
}

void loop()
{
  Serial.println(millis());
  if ( loopEthernet() ) {
  }
  else
  {
    Serial.println("Not connected");   
  }

  loopLights();
}

