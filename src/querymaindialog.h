#ifndef QUERYMAINDIALOG_H
#define QUERYMAINDIALOG_H

#include <QButtonGroup>
#include <QDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QVBoxLayout>

class QueryMainDialog : public QDialog {
    Q_OBJECT
public:
    explicit QueryMainDialog(QWidget *parent = nullptr);
    void addMainWidget(const QString &name);
    bool hasSelectedWidget() const;
    QString selectedWidgetName() const;

private:
    QButtonGroup *m_btnGroup;
    QVBoxLayout *m_contentLayout;
    int m_btnId;
};

#endif // QUERYMAINDIALOG_H
