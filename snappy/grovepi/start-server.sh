export PYTHON_SERVER_HOME="$SNAP_APP_PATH"

LD_LIBRARY_PATH=$PYTHON_SERVER_HOME/lib PYTHON_EGG_CACHE=$SNAP_APP_DATA_PATH/.python-eggs $PYTHON_SERVER_HOME/bin/python-env/bin/python2.7 $PYTHON_SERVER_HOME/bin/grovepi-server.py
