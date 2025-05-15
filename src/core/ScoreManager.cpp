#include "ScoreManager.h"

ScoreManager::ScoreManager() {
    QString homePath = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    dirPath = homePath + "/Game_Project";
    filePath = dirPath + "/scores.json";
    createFile(); //Create the game folder if it doesn't exist (else do nothing)
    actualScore = new Score();
    elapsedTimer = new QElapsedTimer();
}

ScoreManager::~ScoreManager(){
    delete actualScore;
    actualScore = nullptr;
    delete elapsedTimer;
    elapsedTimer = nullptr;
}

void ScoreManager::loadScores(){
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly)){
        createFile();
        return;
    }

    QJsonDocument document = QJsonDocument::fromJson(file.readAll());

    if(document.isArray()){
        QJsonArray scores = document.array();
        bestScoresList.clear();
        for(QJsonValue scoreValue : scores){
            QJsonObject scoreObject = scoreValue.toObject();
            if (scoreObject.contains("date") && scoreObject.contains("score") && scoreObject.contains("timePlayed")) {

                QString date = scoreObject["date"].toString();
                int score = scoreObject["score"].toInt();
                int timePlayed = scoreObject["timePlayed"].toInt();

                Score s(date, score, timePlayed);
                bestScoresList.append(s);
                sortScoresList();
                if(bestScoresList.size() > 5){
                    bestScoresList.pop_front();
                }
            }
        }
    }
    file.close();
}

void ScoreManager::createFile(){
    QDir dir;

    dir.mkpath(dirPath);

    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly)){
        qDebug("Creating scores.json file");
        file.open(QIODevice::WriteOnly);
    }
    file.close();
}

void ScoreManager::addScore(Score score){
    bestScoresList.append(score);
    sortScoresList();
    if(bestScoresList.size() > 5){
        bestScoresList.pop_front();
    }
    saveScores();
}

void ScoreManager::saveScores(){
    QJsonArray scoresArray;
    for(Score score : bestScoresList){
        QJsonObject object;
        object["score"] = score.getScore();
        object["date"] = score.getDate();
        object["timePlayed"] = score.getTimePlayed();
        scoresArray.append(object);
    }

    QJsonDocument document(scoresArray);
    QFile file(filePath);
    file.open(QIODevice::WriteOnly);
    file.write(document.toJson());
    file.close();
}

void ScoreManager::sortScoresList(){
    int n = bestScoresList.size();
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n - i - 1; j++){
            Score* score1 = &(bestScoresList[j]);
            Score* score2 = &(bestScoresList[j + 1]);
            if(bestScoreBetween(score1, score2) == score1){
                std::swap(bestScoresList[j], bestScoresList[j + 1] );
            }
        }
    }
}

Score* ScoreManager::bestScoreBetween(Score* score1, Score* score2){
    if(score1->getScore() > score2->getScore()){
        return score1;
    }else if(score1->getScore() < score2->getScore()){
        return score2;
    } else{
        if(score1->getTimePlayed() < score2->getTimePlayed()){
            return score1;
        } else {
            return score2;
        }
    }
}