/* test.cpp
  Programm to test classes
  Alexey Fokin, 10.09.2021
  trashware
*/

class Shit
{
  public:
    enum class Smell { Good, NotBad, Bad, Nasty, Terrible };
                      
    Shit(float shitVolume, int shitType);
    Smell smell();
    void dry();
  private:
    float _shitVolume;
    int _shitType;
};

Shit::Shit(float shitVolume, int shitType)
{
  _shitVolume = shitVolume;
  _shitType = shitType;
}

Shit::Smell Shit::smell()
{
  if(_shitVolume > 100.0 && _shitType == 2)
  {
    return Smell::Bad;
  }
  return Smell::NotBad; 
}

void Shit::dry()
{
  if(_shitVolume -= 50.0 < 0.0)
    _shitVolume = 0.0;
}

void setup()
{
  Shit pieceOfShit1(100.5, 2);

  pieceOfShit1.dry();
  Shit::Smell sm = pieceOfShit1.smell();
}
void loop() {}
