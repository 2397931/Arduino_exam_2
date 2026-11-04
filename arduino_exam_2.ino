#include <Arduino.h>
#include <Bounce2.h>
#include <MicroOscSlip.h>
#include <Chrono.h>

Bounce2::Button but0;

MicroOscSlip<128> monOsc(&Serial);

Chrono chronoPot;

void setup() {
    Serial.begin(115200);

    but0.attach(8, INPUT_PULLUP);
    but0.setPressedState(LOW);
    pinMode(5, OUTPUT);
}

void traiterMessageOsc(MicroOscMessage &message) {

    // VÉRIFIER L'ADRESSE ET LE TYPE DU MESSAGE
    if (message.checkOscAddressAndTypeTags("/ready", "i")) {

        // RÉCUPÉRER L'ENTIER
        int valeur = message.nextAsInt();

        // ASSIGNER LA VALEUR À LA DEL
        digitalWrite(5, valeur);
    }
}

void loop() {

    monOsc.onOscMessageReceived(traiterMessageOsc); // ACQUISITION DES MESSAGES

    but0.update();

    if (but0.pressed()) { // Bouton vient d’être appuyé
        monOsc.sendInt("/bang", 1);
    }


    if (chronoPot.hasPassed(25))
    {                        // SI LE CHRONO DÉPASSE 20 MILLISECONDES
        chronoPot.restart(); // REPARTIR LE CHRONO

        // LE 3 CI-BAS CORRESPOND À LA BROCHE ANALOGIQUE A1
        int valeur = analogRead(1); // LECTURE DE LA TENSION DE LA BROCHE A1 ENTRE 0 ET 1023

        monOsc.sendInt("/turn", valeur); // ENVOYER LA VALEUR
    }  

}


