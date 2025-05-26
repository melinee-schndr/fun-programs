// Fonction si robot pas alligné vers la lumière
void toLight(int right_light, int left_light) {
    Off(OUT_BC);

    // Si lumière à gauche -> Oriente le robot vers la gauche
    if (left_light > right_light) {
        OnRev(OUT_C, 100);

    // Si lumière à droite -> Oriente le robot vers la droite
    } else {
        OnRev(OUT_B, 100);
    }
}


// Fonction si deux capteurs de pression sont activés (obstacle pile devant)
void bothTouched() {
    OnFwdSync(OUT_BC, 100, 0);
    Wait(300);
    Off(OUT_BC);
    OnRevSyncEx(OUT_BC, 50, 100, 1);
    Wait(300);
    Off(OUT_BC);
}


// Fonction si un seul des deux capteurs de pression est activés (obstacle oblique)
void Touched(int side) {

    // side = -1 -> Left
    // side = 1  -> Right

    Off(OUT_BC);

    if (side < 0) {
        OnFwd(OUT_C, 100);
    } else {
        OnFwd(OUT_B, 100);
    }

    Wait(400);
    OnRevSync(OUT_BC, 60, 0);
    Wait(1400);
}


// Fonction pour rechercher la lumière (si le robot est perdu) -> Robot fait un tour complet sur lui même et s'arrête tous les ~45° pour vérifier la lumière ambiante
void searchLight(int left_light) {
    int i;
    int lights[] = {0, 0, 0, 0, 0, 0, 0, 0};
    int max = 0;
    int max_id = 0;

    // Boucle 8 repérages de lumière
    for(i = 0; i < 8; i++) {
        // Arrêt moteur
        Off(OUT_BC);
        // Update valeur du capteur de lumière de gauche + update du tableau de valeurs de lumière
        left_light = Sensor(S4);
        lights[i] = left_light;

        // Si lumière en face est la meilleure de toutes les autres -> Nouveau max 
        if (left_light > max) {
            max = left_light;
            max_id = i;
        }
        // On tourne vers la droite
        OnFwd(OUT_C, 100);
        OnRev(OUT_B, 100);
        Wait(140);

        // Affichage tableau de valeurs + maximum actuel
        NumOut(0, LCD_LINE5, max_id);
        NumOut(12 * i + 3, LCD_LINE6, left_light);
    }

    // Arrêt moteur + rotation vers la droite
    Off(OUT_BC);
    Wait(200);
    OnFwd(OUT_C, 100);
    OnRev(OUT_B, 100);

    // RJusqu'à l'angle où la lumière était la meilleure puis arrêt moteur (= robot bien orienté vers la lumière)
    Wait(140 * max_id);
    Off(OUT_BC);
    Wait(100);

}


// Fonction pour vérifier si lumière en face ou non, et orienter le robot en conséquence
void checkLight(int right_light, int left_light) {

    // Si lumière pas en face
    if (abs(left_light - right_light) > 25) {
        toLight(right_light, left_light);

    // Si lumière en face
    } else {
        OnRevSync(OUT_BC, 70, 0);
    }
}


// Fonction pour vérifier si au moins un des deux capteurs de pression est activé -> Action en conséquence
int checkTouch(int left_touch, int right_touch) {
    if (right_touch || left_touch){
            Off(OUT_BC);

            // Si les deux activés
            if (right_touch && left_touch) {
                bothTouched();

            // Si uniquement gauche activé
            } else if (left_touch) {
                Touched(-1);

            // Si uniquement droit activé
            } else {
                Touched(1);
            }
       return 1;
    }

    return 0;
}


// Fonction principale
task main() {
    // Initialisation des variables
    int right_light, left_light;
    int right_touch, left_touch;
    int isTouching;

    // Capteurs de lumière
    SetSensorType(S1, SENSOR_TYPE_LIGHT_INACTIVE);
    SetSensorMode(S1, SENSOR_MODE_PERCENT);
    SetSensorType(S4, SENSOR_TYPE_LIGHT_INACTIVE);
    SetSensorMode(S4, SENSOR_MODE_PERCENT);

    // Capteurs de Pression
    SetSensorType(S2, SENSOR_TYPE_TOUCH);
    SetSensorMode(S2, SENSOR_MODE_BOOL);
    SetSensorType(S3, SENSOR_TYPE_TOUCH);
    SetSensorMode(S3, SENSOR_MODE_BOOL);

    // Recherche de lumière initiale pour se diriger vers la lampe (si la salle est pas ensoleillée mdr imagine genre y'a trop de soleil et le robot il fait n'importe quoi (: 
    searchLight(left_light);

    
    // Boucle indéfinie
    while (1) {

        // Update des valeurs des capteurs de lumière
        right_light = Sensor(S1);
        left_light = Sensor(S4);

        // Update des valeurs des capteurs de pression
        right_touch = Sensor(S2);
        left_touch = Sensor(S3);


        // Booléenne pour savoir si le robot a touché un obstacle (si oui, il aura agit en conséquence)
        isTouching = checkTouch(left_touch, right_touch);

        // S'il n'a pas touché d'obstacle -> Il se dirige vers la lumière
        if (!isTouching) {
            checkLight(right_light, left_light);
        }
        
        // Si plus assez de lumière tout droit -> Recherche d'une meilleur source de lumière
        if (left_light + right_light < 50) {
            searchLight(left_light);
        }


        // Affichages valeurs des capteurs de lumière
        NumOut(0, LCD_LINE3, right_light);
        NumOut(60, LCD_LINE3, left_light);

        // Affichages valeurs des capteurs de pression
        NumOut(0, LCD_LINE4, right_touch);
        NumOut(60, LCD_LINE4, left_touch);
    }
}
