#include "FilesEditor.h"



FilesEditor::FilesEditor(QWidget *parent)
    : QDialog(parent), m_impl(new EditImpl), m_operation(new XOR)
{
    m_impl->init(this);
    m_impl->connect();


    connect(m_impl.data(), &Impl::saveMask, this, &FilesEditor::setMask );
    connect(m_impl.data(), &Impl::savePath, this, &FilesEditor::setSavePath );
    connect(m_impl.data(), &Impl::openFile, this, &FilesEditor::setOpenPath );

    connect(m_impl.data(), &Impl::operationChanged, this, &FilesEditor::createOperation );
    connect(m_impl.data(), &Impl::transform, this, &FilesEditor::transformFile);
}

void FilesEditor::createOperation(Impl::Operations operation)
{
    switch (operation)
    {
    case Impl::Operations::OR:
        m_operation.reset(new OR);
        break;

    case Impl::Operations::AND:
        m_operation.reset(new AND);
        break;

    case Impl::Operations::NOT:
        m_operation.reset(new NOT);
        break;

    case Impl::Operations::XOR:
        m_operation.reset(new XOR);
        break;

    case Impl::Operations::SHLD:
        m_operation.reset( new SHLD);
        break;

    case Impl::Operations::SHRD:
        m_operation.reset( new SHRD);
        break;

    }
}

void FilesEditor::transformFile(const QString& operand, bool isDelete)
{

    double key;
    std::istringstream(operand.toStdString()) >> std::hex >> key;

    uint64_t* hash = (uint64_t*)&key;

    QFile outFile(m_openPath);

    QFileInfo info(m_openPath);

    if(info.isExecutable())
    {
        QMessageBox::warning(nullptr,"", "Input file is executing!");
        return;
    }

    if(!outFile.exists())
    {
        QMessageBox::warning(nullptr,"", "No input file");
        return;
    }

    if(!outFile.open(QIODevice::OpenModeFlag::ReadOnly))
    {
        QMessageBox::warning(nullptr,"", "Not open");
        return;
    }

    QFile inFile(m_savePath);

    if(inFile.isOpen())
    {
        QMessageBox::warning(nullptr,"", "Output file is open!");
        return;
    }

//    if(!inFile.exists())
//    {
//        QMessageBox::warning(nullptr,"", "No output file");
//        return;
//    }

    if(!inFile.open(QIODevice::OpenModeFlag::WriteOnly))
    {
        QMessageBox::warning(nullptr,"", "Not open");
        return;
    }


    QByteArray information = outFile.readAll();
   m_operation->transform(information, hash);

    inFile.write(information);
    QMessageBox::information(nullptr,"", "Ready!");

    inFile.close();
    outFile.close();

    if(isDelete)
        outFile.remove();
}


FilesEditor::~FilesEditor()
{
}

