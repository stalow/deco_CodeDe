// auteur : Hicham
//fct page config 


#include "./headers/Init_Config.h"
#include "./headers/Eeprom_Controller.h"
#include "./headers/State_Flags.h"
#include "./headers/settings.h"

AsyncWebServer server(80);
String ssid;
String password;
String SerieNumber;

bool checkCredentials(const String &ssid, const String &password)
{
    char count = 0;
    /*if (!isSafeString(ssid) || !isSafeString(password))
    {
        DEBUG_PRINTLN("Credentials not valid");
        return false; // Caractères suspects détectés
    }*/

    WiFi.begin(ssid.c_str(), password.c_str());
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        DEBUG_PRINTLN(".");
        count++;
        if (count >= CONNECTION_MAX_DELAY_WAIT)
        {
            DEBUG_PRINTLN("Credentials not connecting");
            return false;
        }
    }
    DEBUG_PRINTLN("Credentials valid");
    return true;
}

// Fonction pour vérifier si la chaîne ne contient pas de caractères suspects
bool isSafeString(const String &str)
{
    for (size_t i = 0; i < str.length(); i++)
    {
        if (str[i] != '_' && str[i] != '-' && 1)
        {
            return false;
        }
    }
    return true;
}

String readHtmlFile(const char *path)
{
    File file = SPIFFS.open(path, "r");
    if (!file)
    {
        DEBUG_PRINTLN("Impossible d'ouvrir le fichier");
        return String();
    }

    String content = file.readString();
    file.close();
    return content;
}

void setupWiFiAP()
{
    WiFi.softAP("MonAP", ""); // Remplacez par le SSID et le mot de passe de votre choix
    DEBUG_PRINTLN("Point d'accès WiFi créé");

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              {
                  DEBUG_PRINTLN("Get WIFI form Page");
                  String htmlContent = readHtmlFile("/index.html"); // Remplacez par le chemin de votre fichier HTML
                  request->send(200, "text/html", htmlContent); });

    server.on("/submit", HTTP_POST, [](AsyncWebServerRequest *request)
              {
                int index_wifi_storage = 0;
                DEBUG_PRINTLN("submit form");

                // Vérifier si le type de contenu est "application/x-www-form-urlencoded"
                if(request->contentType() == "application/x-www-form-urlencoded"){
                    // Récupérer les données du formulaire
                     ssid = request->arg("ssid");
                     password = request->arg("password");

                    DEBUG_PRINTLN("SSID: " + ssid);
                    DEBUG_PRINTLN("Password: " + password);

                    // Vérifier les informations (vous devrez implémenter votre propre logique de vérification)
                    if (checkCredentials(ssid, password))
                    {
                        index_wifi_storage = getNumPairs();
                        saveWiFiCredentials(ssid,password,index_wifi_storage);
                        DEBUG_PRINTLN("wifi ok, go page config");
                        String successHtmlContent = readHtmlFile("/page_config_2.html");
                        request->send(200, "text/html", successHtmlContent);
                    }
                    else
                    {
                        DEBUG_PRINTLN("Echec wifi, error");
                        String errorHtmlContent = readHtmlFile("/index.html");
                        errorHtmlContent.replace("<!--{error_message}-->", "Échec de la connexion. Veuillez réessayer.");
                        request->send(200, "text/html", errorHtmlContent);
                        //deleteInitFile();
                    }
                }
                else
                {
                    DEBUG_PRINTLN("Mauvais type de contenu");
                    request->send(400); // Mauvaise requête si le type de contenu n'est pas pris en charge
                } });

    /*server.on("/config", HTTP_GET, [](AsyncWebServerRequest *request)
              {
                  DEBUG_PRINTLN("Get conf num serie");
                  String htmlContent = readHtmlFile("/page_config_2.html");
                  request->send(200, "text/html", htmlContent); });*/

    server.on("/submitNum", HTTP_POST, [](AsyncWebServerRequest *request)
              {
                DEBUG_PRINTLN("Get num serie");
                SerieNumber = request->arg("numeroSerie");
                request->send(200, "text/html", "ok");
                delay(500);
                deleteInitFile();
                stopServerAndDisconnectWiFi();
                FLAG_END_FIRST_START_CONFIG = true ;
                return 0; });

    server.begin();
    DEBUG_PRINTLN("Serveur web démarré");
    FLAG_PAGE_WIFI_STARTED = true;
}

bool checkInitFile()
{   
    listFiles(); // voir tous les fichier dispo 
    // Vérifier si le fichier "init.txt" existe
    if (SPIFFS.exists("/init.txt"))
    {
        DEBUG_PRINTLN("Le fichier init.txt existe.");
        return true;
    }
    else
    {
        DEBUG_PRINTLN("Le fichier init.txt n'existe pas.");
        return true;
    }
}

void deleteInitFile()
{
    // Supprimer le fichier "init.txt"
    if (SPIFFS.remove("/init.txt"))
    {
        DEBUG_PRINTLN("Le fichier init.txt a été supprimé avec succès.");
    }
    else
    {
        DEBUG_PRINTLN("Erreur lors de la suppression du fichier init.txt.");
    }
}

void stopServerAndDisconnectWiFi()
{
    // Arrêt du serveur web
    server.end();
    DEBUG_PRINTLN("Serveur web arrêté");

    // Déconnexion du Wi-Fi
    WiFi.disconnect(true);
    DEBUG_PRINTLN("Déconnecté du Wi-Fi");
}