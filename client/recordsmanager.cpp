#include "recordsmanager.h"
#include <QStandardPaths>

RecordsManager* RecordsManager::_instance = nullptr;

RecordsManager *RecordsManager::Instance(QObject *parent)
{
    if (_instance == nullptr)
    {
        _instance = new RecordsManager(parent);
    }

    return _instance;
}

RecordsManager::~RecordsManager()
{
    stopRecording();
    delete _textStream;
    delete _fileHandler;
}

RecordsManager::RecordsManager(QObject *parent)
    :
    QObject(parent),
    _recordsDir(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + QStringLiteral(u"/DCS_SA/")),
    _fileHandler(new QFile),
    _textStream(new QTextStream)
{
    _textStream->setDevice(_fileHandler);
    _messagesSeparator = "\n";

    if (!_recordsDir.exists())
    {
        _recordsDir.mkdir(_recordsDir.absolutePath());
    }
}

void RecordsManager::clear()
{
    _recordedMessages.clear();
}

QString RecordsManager::getRecordsPath() const
{
    return _recordsDir.absolutePath();
}

QStringList RecordsManager::getRecords() const
{
    return _recordsDir.entryList();
}

void RecordsManager::loadRecord(const QString &filePath)
{
    stopRecording();

    _recordedMessages.clear();
    _fileHandler->setFileName(filePath);

    if (_fileHandler->open(QIODevice::ReadOnly))
    {
        while (!_textStream->atEnd())
        {
            _recordedMessages.append(_textStream->readLine().toUtf8());
        }
    }
}

void RecordsManager::unloadRecord()
{
    _recordedMessages.clear();
}

int RecordsManager::countLoadedRecordMessages()
{
    return _recordedMessages.count();
}

void RecordsManager::initNewRecord(const QString &name)
{
    if (_isRecording)
    {
        return;
    }

    _isRecording = false;

    _fileHandler->close();
    _fileHandler->setFileName(_recordsDir.absolutePath() + "/" + name + ".txt");

    if (_fileHandler->open(QIODevice::WriteOnly))
    {
        _isRecording = true;
    }
}

void RecordsManager::stopRecording()
{
    _isRecording = false;
    _fileHandler->close();
}

void RecordsManager::selectMessage(int num)
{
    if (num >= _recordedMessages.count() || num < 0)
    {
        return;
    }

    emit sendMessage(_recordedMessages.at(num));
}

void RecordsManager::receiveInfo(const QByteArray &json)
{
    if (!_isRecording || !_fileHandler->isOpen())
    {
        return;
    }

    *_textStream << json << _messagesSeparator;
}
