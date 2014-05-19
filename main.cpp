#include <QCoreApplication>

#include "outputwriter.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    // Test outputwriter
    OutputWriter writer("test.out");
    QVector<DataRow> data{DataRow(1, 0.5)};
    writer.write(data);
    //return a.exec(); // For now we don't need the event loop, only necessary for threads, signal/slots, timers, etc.
}
