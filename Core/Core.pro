TEMPLATE = subdirs

SUBDIRS += \
    callback \
    dependencymanager \
    serializer \
    variant \
    lazyevaluation \
    lazyconditionwalker \
    runtimemetaobject \
    qtmetaobjectwrapper \
    jsonmetaobjectreader \
    temporarybufferdevice \
    referencedevice \
    session

unix: SUBDIRS += unixsignalhandler
