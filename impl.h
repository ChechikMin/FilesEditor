#ifndef IMPL_H
#define IMPL_H

#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QRadioButton>
#include <QDialogButtonBox>
#include <QMessageBox>
#include <QFileDialog>
#include <QTimer>
#include <bitset>

class Impl : public QObject
{
    Q_OBJECT
public:

    virtual void init(QWidget* p) = 0;
    virtual void connect() = 0;
    virtual ~Impl() = default;


    enum class Operations {XOR = 0,
                           NOT = 1,
                           AND = 2,
                           OR = 3,
                           SHLD = 4,
                           SHRD = 5};
signals:
    void saveMask(const QString&);
    void openFile(const QString&);
    void savePath(const QString&);
    void operationChanged(Operations);
    void transform(const QString&, bool isDelete);
};

class EditImpl : public Impl
{
    Q_OBJECT
public:
    virtual void init(QWidget* p) override;
    virtual void connect() override;

private:
    QVBoxLayout* m_layout;

    QLineEdit* m_input;
    QLabel* m_infoInput;
    QPushButton* m_setInput;

    QLineEdit* m_template;
    QLabel* m_infoTemplate;
    QPushButton* m_saveTemplate;

    QCheckBox* m_isDeleteInput;

    QLineEdit* m_output;
    QLabel* m_infoDeleteInput;
    QPushButton* m_setOutput;

    QLabel* m_editFileRule;
    QRadioButton* m_overwrite;
    QRadioButton* m_addNew;


    QLabel* m_workMode;
    QCheckBox* m_isTimer;
    QSpinBox* m_timerScale;

    QLabel* m_editMaskInfo;
    QLineEdit* m_editValue;
    QComboBox* m_operations;

    QPushButton* m_process;
    QPushButton* m_stop;
    QTimer *m_timer;

    const QStringList operations {"XOR",
                                  "NOT",
                                  "AND",
                                  "OR",
                                  "SHLD",
                                  "SHRD"};
};


#endif // IMPL_H
