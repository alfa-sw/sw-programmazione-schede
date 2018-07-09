#ifndef PROGRAMSETTING_H
#define PROGRAMSETTING_H

#include <QObject>
#include <QHash>

typedef struct
{
    bool programmaMAB;
    QString nomeFileMAB;
    bool programmaAttuatori[32];
    QString nomeFileAttuatori;
    bool programmaAssi[2];
    QString nomeFileAssi;
    bool programmaContainer[4];
    QString nomeFileContainer;
    bool programmaCover[2];
    QString nomeFileCover;
    bool programmaAutocap;
    QString nomeFileAutocap;
    bool programmaCanLifter;
    QString nomeFileCanLifter;
    bool programmaHumidifier;
    QString nomeFileHumidifier;

} STRUTTURA_PROGRAMMA;

class programSetting
{
public:
    programSetting(STRUTTURA_PROGRAMMA *struttura);
private:
   STRUTTURA_PROGRAMMA *strutturaProgramma;

   void initAll();


};

#endif // PROGRAMSETTING_H
