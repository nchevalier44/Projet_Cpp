#ifndef PROJET_CPP_SCOREMANAGER_H
#define PROJET_CPP_SCOREMANAGER_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QJsonArray>
#include <QElapsedTimer>

class Score{
private:
    QString date = "";
    int score = 0;
    int timePlayed = 0;

public:
    Score(QString d="", int s=0, int t=0) : score(s), date(d), timePlayed(t) {}

    QString getDate() const { return date; }
    int getScore() const { return score; }
    int getTimePlayed() const { return timePlayed; }

    void setScore(int newscore){ score = newscore; }
    void setDate(QString newdate){ date = newdate; }
    void setTimePlayed(int seconds){ timePlayed = seconds; }

};

class ScoreManager {
public:
    ScoreManager();
    ~ScoreManager();

    void createFile();
    void loadScores();
    QList<Score> getBestScoresList() { return bestScoresList; };
    void addScore(Score score);
    void sortScoresList();
    Score* bestScoreBetween(Score* score1, Score* score2);
    void saveScores();

    Score* getActualScore() { return actualScore; };
    void setActualScore(Score* newscore){ actualScore = newscore; }
    QElapsedTimer* getElapsedTimer(){ return elapsedTimer; }

private:
    QList<Score> bestScoresList;
    QString dirPath;
    QString filePath;

    Score* actualScore = nullptr;
    QElapsedTimer* elapsedTimer = nullptr;
};




#endif //PROJET_CPP_SCOREMANAGER_H
