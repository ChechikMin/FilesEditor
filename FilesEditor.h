#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include "impl.h"
#include <QString>
#include <QDebug>
#include <sstream>
#include <bitset>
#include <QProcess>

constexpr size_t size = sizeof (uint64_t) * CHAR_BIT;

class BitOperation
{
public:
    virtual void transform(QByteArray&, const uint64_t * rightOperand) = 0;
    virtual ~BitOperation() = default;
};

class OR : public BitOperation
{


    virtual void transform(QByteArray& arr, const uint64_t * rightOperand)
    {
        size_t i = 0;

        std::transform(arr.begin(), arr.end(),arr.begin(), [&rightOperand, &i](char byte)
        {
            i++;
            return byte | rightOperand[i % size];
        }
        );
    }
};

class XOR : public BitOperation
{
    virtual void transform(QByteArray& arr, const uint64_t * rightOperand)
    {
        size_t i = 0;

        std::transform(arr.begin(), arr.end(),arr.begin(), [&rightOperand, &i](char byte)
        {
            i++;
            return byte ^ rightOperand[i % size];
        }
        );
    }
};

class NOT : public BitOperation
{
    virtual void transform(QByteArray& arr, const uint64_t * rightOperand)
    {
        std::transform(arr.begin(), arr.end(),arr.begin(), [](char byte)
        {
            return ~byte;
        }
        );
    }
};

class AND : public BitOperation
{
    virtual void transform(QByteArray& arr, const uint64_t * rightOperand)
    {
        size_t i = 0;

        std::transform(arr.begin(), arr.end(),arr.begin(), [&rightOperand, &i](char byte)
        {
            i++;
            return byte & rightOperand[i % size];
        }
        );
    }
};

class SHLD : public BitOperation
{
    virtual void transform(QByteArray& arr, const uint64_t * rightOperand)
    {
        size_t i = 0;

        std::transform(arr.begin(), arr.end(),arr.begin(), [&rightOperand, &i](char byte)
        {
            i++;
            return byte << rightOperand[i % size];
        }
        );
    }
};

class SHRD : public BitOperation
{
    virtual void transform(QByteArray& arr, const uint64_t * rightOperand)
    {
        size_t i = 0;

        std::transform(arr.begin(), arr.end(),arr.begin(), [&rightOperand, &i](char byte)
        {
            i++;
            return byte >> rightOperand[i % size];
        }
        );
    }
};



class FilesEditor : public QDialog
{
    Q_OBJECT
    QScopedPointer<Impl> m_impl;
    QScopedPointer<BitOperation> m_operation;
    QString m_filesMask;//???
    QString m_openPath;
    QString m_savePath;

public:

    FilesEditor(QWidget *parent = nullptr);
    void setMask(const QString& mask) {m_filesMask = mask;}
    void setSavePath(const QString& path){m_savePath = path;}
    void setOpenPath(const QString& path){m_openPath = path;}
    void createOperation(Impl::Operations);
    void transformFile(const QString& value, bool isDelete);
    ~FilesEditor();
};


#endif // DIALOG_H
