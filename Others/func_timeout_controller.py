#!/usr/bin/env python
# -*- coding: utf-8 -*-
from functools import wraps
import os

def func_timeout_controller(timeout,exception):
	from gevent import Timeout
	@wraps(timeout,exception)
	def _decorator(*args,**kwargs):
		try: 
			timer = Timeout.start_new(timeout,exception)
			timer.start()
			ret = func(*args,**kwargs)