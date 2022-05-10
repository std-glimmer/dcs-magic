#ifndef RECORDSMANAGER_H
#define RECORDSMANAGER_H

#include <QString>
#include <QObject>
#include <QDir>
#include <QTextStream>
#include <QByteArray>
#include <QVector>

class RecordsManager : public QObject
{
    Q_OBJECT
public:
    static RecordsManager *Instance(QObject* parent = nullptr);
    ~RecordsManager();

    RecordsManager(RecordsManager &other) = delete;
    void operator=(const RecordsManager &) = delete;

    void clear();

    QString getRecordsPath() const;
    QStringList getRecords() const;

    void loadRecord(const QString& filePath);
    void unloadRecord();

    int countLoadedRecordMessages();

    void initNewRecord(const QString& name);
    void stopRecording();

public slots:
    void selectMessage(int num);
    void receiveInfo(const QByteArray& json);

signals:
    void sendMessage(const QByteArray& message);

private:
    explicit RecordsManager(QObject* parent = nullptr);
    static RecordsManager* _instance;

    QDir _recordsDir;

    // Запись треков
    QFile* _fileHandler = nullptr;
    QTextStream* _textStream = nullptr;
    QString _messagesSeparator;

    // Проигрывание треков
    QVector<QByteArray> _recordedMessages;

    bool _isRecording = false;
};

#endif // RECORDSMANAGER_H
