int Value = A8; 

//Variable pour le filtrage
int valeur_1 = 0;
int valeur_2 = 0;
int valeur_sortie = 0;

//Variable pour détecter la monté et la descente
int valeur_precedente = 0;
bool haut_bas = 1; //0 si en bas 1 si en haut

//Temps de monté et de descente pour la lecture des bits
unsigned long temps_d = 0;
unsigned long temps_m = 0;
unsigned long temps = 0;
unsigned long save = 0; //Permet de calculer le temps entre 2 descentes
int compteur_descente = 0;

//Bit de test entre 2 descente
unsigned long temps_d2 = 0;

// Stockage et décodage de la trame.
unsigned int seconde_tab[10];
int seconde =0;
int indice_bit = 0;

int bit = 0;

int seuil = 560; 

void setup() {
  Serial.begin(9600);
  pinMode(Value, INPUT);
}

void loop() 
{
  Value = analogRead(A8);
    
  valeur_sortie = (Value + valeur_1 + valeur_2)/3;
  valeur_2 = valeur_1;
  valeur_1 = Value;

  //Serial.print(valeur_sortie);
  //Serial.println();
  
  //Test de la descente
  if (haut_bas == 1 && valeur_sortie < seuil)
  {
    save = micros();
    temps_d2 = temps_d;
    temps_d = save;

    //Serial.print("Le temps entre 2 descente : ");
    //Serial.println(temps_d - temps_d2);
    haut_bas = 0;

    if(temps_d - temps_d2 > 1500)
    {
      //Serial.println(compteur_descente);
      if (compteur_descente == 4)
      {
        Serial.println("Bit à 0");
        bit = 0;
        seconde_tab[indice_bit] = 0;
        indice_bit = indice_bit + 1;
      }

      else if (compteur_descente == 10)
      {
        Serial.println("Bit à 1");
        bit = 1;
        seconde_tab[indice_bit] = 1;
        indice_bit = indice_bit + 1;
      }

      if (indice_bit == 8){
        for (int i =0; i < 8;i ++){
          Serial.print(seconde_tab[indice_bit]);
        }
        Serial.println();
        indice_bit = 0;
      }
          
      else if(compteur_descente != 4 && compteur_descente != 10) {
        Serial.println("Bit de positionnement");
        bit = 0;
      }
         
      compteur_descente = 0;
    }


    compteur_descente = compteur_descente + 1;
  }

  //Test de la montée
  if ( haut_bas == 0 && valeur_sortie > seuil)
  {
    
    temps_m = micros();
    haut_bas = 1;

    //Serial.print("Ecart de temps monté descente : ");
    //Serial.println(temps_m - temps_d);

    //Serial.println(millis());  
  }
  valeur_precedente = valeur_sortie;
}

