#ifndef BDMODEL_H
#define BDMODEL_H
#include <QFileSystemModel>

class FileExplorerModel: public QFileSystemModel
{
public:
    FileExplorerModel(QObject *parent = nullptr);
};
#endif // FILEEXPLORERMODEL_H
