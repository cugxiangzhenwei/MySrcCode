#include <QtGui>

#include "configdialog.h"
#include "baseinfo.h"
#include "detail.h"
#include "contact.h"


ConfigDialog::ConfigDialog()
{
    contentsWidget = new QListWidget;
    contentsWidget->addItem(tr("base info"));
    contentsWidget->addItem(tr("contact"));
    contentsWidget->addItem(tr("detail"));
    contentsWidget->setMaximumWidth(128);
    connect(contentsWidget,
            SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)),
            this, SLOT(changePage(QListWidgetItem *, QListWidgetItem*)));

    pagesWidget = new QStackedWidget;
    pagesWidget->addWidget(new BaseInfo);
    pagesWidget->addWidget(new Contact);
    pagesWidget->addWidget(new Detail);
    
    connect(pagesWidget, SIGNAL(currentChanged(int)),
                this, SLOT(fadeInWidget(int)));

    QPushButton *closeButton = new QPushButton(tr("Close"));

 //   createIcons();
    contentsWidget->setCurrentRow(0);

    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));

    QHBoxLayout *horizontalLayout = new QHBoxLayout;
    horizontalLayout->addWidget(contentsWidget);
    horizontalLayout->addWidget(pagesWidget, 1);

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(closeButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(horizontalLayout);
    mainLayout->addStretch(1);
    mainLayout->addSpacing(12);
    mainLayout->addLayout(buttonsLayout);
    setLayout(mainLayout);

    setWindowTitle(tr("Fade in fade out example"));
    
    faderWidget = new FaderWidget(this);
    faderWidget->start();
}

void ConfigDialog::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current)
        current = previous;

    pagesWidget->setCurrentIndex(contentsWidget->row(current));
}

void ConfigDialog::fadeInWidget(int index)
    {
        if (faderWidget)
            faderWidget->close();
        faderWidget = new FaderWidget(
                                  pagesWidget->widget(index));
        faderWidget->start();
    }

