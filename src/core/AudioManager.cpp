#include <iostream>
#include "AudioManager.h"

//Add QAudioOuput to the list
void AudioManager::addMusicObject(QAudioOutput* sound, int initialVolume){
    musicObjectMap.insert(sound, initialVolume);
    sound->setVolume(float(initialVolume) * float(musicVolumePercentage) / 100);
}

//Add QSoundEffect to the list
void AudioManager::addSFXObject(QSoundEffect* sound, int initialVolume){
    SFXObjectMap.insert(sound, initialVolume);
    sound->setVolume(float(initialVolume) * float(SFXVolumePercentage) / 100);
}

//Update all QAudioOutput volume
void AudioManager::updateMusicVolume(){
    for(auto iterator= musicObjectMap.begin(); iterator != musicObjectMap.end(); iterator++){
        QAudioOutput* sound = iterator.key();
        int initialVolume = iterator.value();
        sound->setVolume(float(initialVolume) * float(musicVolumePercentage) / 100);
    }
}

//Update all QSoundEffect volume
void AudioManager::updateSFXVolume(){
    for(auto iterator= SFXObjectMap.begin(); iterator != SFXObjectMap.end(); iterator++){
        QSoundEffect* sound = iterator.key();
        int initialVolume = iterator.value();
        sound->setVolume(float(initialVolume) * float(SFXVolumePercentage) / 100);
    }
}