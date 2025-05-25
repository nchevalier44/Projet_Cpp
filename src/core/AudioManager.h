#ifndef PROJET_CPP_AUDIOMANAGER_H
#define PROJET_CPP_AUDIOMANAGER_H

#include <QMap>
#include <QSoundEffect>
#include <QAudioOutput>

class AudioManager {
private:
    QMap<QAudioOutput*, float> musicObjectMap;
    QMap<QSoundEffect*, float> SFXObjectMap;

    int musicVolumePercentage = 100;
    int SFXVolumePercentage = 100;
public:

    //Getters
    int getMusicVolumePercentage() const { return musicVolumePercentage; }
    int getSFXVolumePercentage() const { return SFXVolumePercentage; }

    //Setters
    void setMusicVolumePercentage(int newVolume) { musicVolumePercentage = newVolume; }
    void setSFXVolumePercentage(int newVolume) { SFXVolumePercentage = newVolume; }

    //Update functions
    void updateMusicVolume();
    void updateSFXVolume();

    //Add functions
    void addSFXObject(QSoundEffect* sound, float initialVolume);
    void addMusicObject(QAudioOutput* sound, float initialVolume);
};


#endif //PROJET_CPP_AUDIOMANAGER_H
