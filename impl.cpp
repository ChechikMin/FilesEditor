#include "impl.h"

void EditImpl::init(QWidget* p)
{
    m_layout = new QVBoxLayout { p };

    m_isDeleteInput = new QCheckBox{"Delete input file",p};


    m_infoInput = new QLabel{"Path to file", p};
    m_setInput = new QPushButton{"Choose", p};
    m_input = new QLineEdit { p };
    m_input->setEnabled(false);


    QHBoxLayout* layoutPath = new QHBoxLayout();

    layoutPath->addWidget(m_infoInput);
    layoutPath->addWidget(m_input);
    layoutPath->addWidget(m_setInput);


    QHBoxLayout* layoutTemplate = new QHBoxLayout();

    m_infoTemplate = new QLabel {"Files mask", p};
    m_template = new QLineEdit {p};
    m_saveTemplate = new QPushButton {"Save", p};

    layoutTemplate->addWidget(m_infoTemplate);
    layoutTemplate->addWidget(m_template);
    layoutTemplate->addWidget(m_saveTemplate);

    QHBoxLayout* layoutOutput = new QHBoxLayout();

    m_infoDeleteInput = new QLabel {"Output path", p};
    m_output = new QLineEdit { p };
    m_setOutput = new QPushButton {"Choose", p};
    m_output->setEnabled(false);

    layoutOutput->addWidget(m_infoDeleteInput);
    layoutOutput->addWidget(m_output);
    layoutOutput->addWidget(m_setOutput);

    QHBoxLayout* layoutOverwrite = new QHBoxLayout();
    m_editFileRule = new QLabel {"If file exist:", p};
    m_overwrite = new QRadioButton{"Overwrite", p};
    m_addNew = new QRadioButton{"Add new", p};;

    layoutOverwrite->addWidget(m_editFileRule);
    layoutOverwrite->addWidget(m_overwrite);
    layoutOverwrite->addWidget(m_addNew);

    QHBoxLayout* layoutTimer = new QHBoxLayout();
    m_workMode = new QLabel {"Work mode\n(one time default)", p};
    m_isTimer = new QCheckBox{"Timer", p};
    m_timerScale = new QSpinBox{p};

    m_timerScale->setMinimum(1);

    m_timerScale->setSingleStep(1);
    m_timerScale->setEnabled(false);

    layoutTimer->addWidget(m_workMode);
    layoutTimer->addWidget(m_isTimer);
    layoutTimer->addWidget(m_timerScale);

    QHBoxLayout* layoutMask = new QHBoxLayout();
    m_editMaskInfo = new QLabel {"Mask operation(in hex)", p};
    m_operations = new QComboBox{p};
    m_editValue = new QLineEdit{p};

    m_operations->addItems(operations);

    layoutMask->addWidget(m_editMaskInfo);
    layoutMask->addWidget(m_operations);
    layoutMask->addWidget(m_editValue);

    QHBoxLayout* layoutHandle = new QHBoxLayout();
    m_process = new QPushButton{"Run",p};
    m_stop = new QPushButton{"Stop",p};
    m_timer = new QTimer{m_process};

    layoutHandle->addWidget(m_process);
    layoutHandle->addWidget(m_stop);

    m_layout->addWidget(m_isDeleteInput);
    m_layout->addLayout(layoutTemplate);
    m_layout->addLayout(layoutPath);
    m_layout->addLayout(layoutOutput);
    m_layout->addLayout(layoutOverwrite);
    m_layout->addLayout(layoutTimer);
    m_layout->addLayout(layoutMask);
    m_layout->addLayout(layoutHandle);

    p->setLayout(m_layout);
}
void EditImpl::connect()
{
    QObject::connect(m_setInput, &QPushButton::clicked, this ,[this]()
    {
        if(m_template->text().isEmpty())
        {
            QMessageBox msgBox;
            msgBox.setText("Set mask!");
            msgBox.exec();
            return;
        }

        QString path = QFileDialog::getOpenFileName(nullptr, "", "", m_template->text());

        m_input->setText(path);
        emit openFile(path);
        emit saveMask(m_template->text());
    } );

    ///////////////////////////////////////
    QObject::connect(m_setOutput, &QPushButton::clicked, this ,[this]()
    {
        QFileDialog fd;

        if(m_addNew->isChecked())
            fd.setAcceptMode(QFileDialog::AcceptSave);

        else if(m_overwrite->isChecked());
        else
        {
            QMessageBox::warning(nullptr,"", "Choose file exist option");
            return;
         }
        fd.exec();

        if(fd.selectedFiles().isEmpty())
            return;

        QString path = fd.selectedFiles()[0];

        if(m_addNew->isChecked())
        {
            QFileInfo info(path);
            QString newName = info.baseName() + "_1" + info.suffix();
            path = info.absoluteFilePath() + newName;
        }

        m_output->setText(path);
        emit savePath(path);


    } );
    ////////////////////////////////////

    QObject::connect(m_operations, &QComboBox::currentTextChanged, this ,[this](const QString & operation)
    {
        emit operationChanged(Operations(m_operations->findText(operation)));

    } );
    ///////////////////////////////////
    QObject::connect(m_isTimer, &QCheckBox::clicked, m_timerScale ,&QCheckBox::setEnabled);

    ///////////////////////////////////
    QObject::connect(m_process, &QPushButton::clicked, this ,[this]()
    {
        if(m_isTimer->isChecked())
        {
            QObject::connect(m_timer, &QTimer::timeout, this, [this](){ emit transform(m_editValue->text(), m_isDeleteInput->isChecked()); });
            m_timer->start(m_timerScale->value() * 1000);

            m_process->setEnabled(true);//write if timer and think about block when processing
        }

        else emit transform(m_editValue->text(), m_isDeleteInput->isChecked());

    } );

    QObject::connect(m_stop, &QPushButton::clicked, m_timer , &QTimer::stop);


}
