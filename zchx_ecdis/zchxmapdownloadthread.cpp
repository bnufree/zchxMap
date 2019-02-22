#include "zchxmapdownloadthread.h"
#include "zchxmapdownloadrunfunction.h"
#include <QThreadPool>

namespace qt {
zchxMapDownloadThread::zchxMapDownloadThread(QObject *parent) : QThread(parent)
{

}

void zchxMapDownloadThread::run()
{
    QThreadPool pool;
    pool.setMaxThreadCount(16);
    for(int i=5; i<23; i++)
    {
        zchxMapRange range {5.50501, 66.901742, 56.224377, 144.170839};
        zchxMapDownloadRunFunction *fun = new zchxMapDownloadRunFunction(range, i);
        connect(fun, SIGNAL(signalDownloadFinished(int)), this, SIGNAL(signalDownloadFinished(int)));
        pool.start(fun);
    }
    pool.waitForDone();
}
}
