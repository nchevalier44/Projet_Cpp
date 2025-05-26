#include "AudioManager.h"

//Add QAudioOuput to the list
void AudioManager::addMusicObject(QAudioOutput* sound, float initialVolume){
    musicObjectMap.insert(sound, initialVolume);
    sound->setVolume(float(initialVolume) * float(musicVolumePercentage) / 100);
    QObject::connect(sound, &QSoundEffect::destroyed, sound, [this, sound]() {
        musicObjectMap.remove(sound); //if destroyed, we remove the sound of the list to avoid using invalid pointer
    });
}

//Add QSoundEffect to the list
void AudioManager::addSFXObject(QSoundEffect* sound, float initialVolume){
    SFXObjectMap.insert(sound, initialVolume);
    sound->setVolume(float(initialVolume) * float(SFXVolumePercentage) / 100);
    QObject::connect(sound, &QSoundEffect::destroyed, sound, [this, sound]() {
        SFXObjectMap.remove(sound); //if destroyed, we remove the sound of the list to avoid using invalid pointer
    });
}

//Update all QAudioOutput volume in function of volume set in settings widget
void AudioManager::updateMusicVolume(){
    for(auto iterator= musicObjectMap.begin(); iterator != musicObjectMap.end(); iterator++){
        QAudioOutput* sound = iterator.key();
        float initialVolume = iterator.value();
        if(sound) sound->setVolume(float(initialVolume) * float(musicVolumePercentage) / 100);
    }
}

//Update all QSoundEffect volume in function of volume set in settings widget
void AudioManager::updateSFXVolume(){
    for(auto iterator= SFXObjectMap.begin(); iterator != SFXObjectMap.end(); iterator++){
        QSoundEffect* sound = iterator.key();
        float initialVolume = iterator.value();
        if(sound) sound->setVolume(float(initialVolume) * float(SFXVolumePercentage) / 100);
    }
}