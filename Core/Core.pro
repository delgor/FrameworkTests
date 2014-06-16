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
    referencedevice

unix: SUBDIRS += unixsignalhandler
