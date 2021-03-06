var LibraryJsGUI  = {
    $JsGUI: {
	},
	CreateCanvas : function(thewindow,isfirst)
	{
		var windowname=UTF8ToString(thewindow);
		//Module.canvas.parentElement.style.position = "relative";
		if(isfirst==0)
		{
			var newcanvas = document.createElement('canvas');
			Module.canvas.parentElement.appendChild(newcanvas);
			newcanvas.id = windowname;
			newcanvas.style.position = "absolute";
			newcanvas.absolutePositionning = 1;
		}
		else
		{
			var canvas = Module['canvas'];
			canvas.id = windowname;
			canvas.style.position = "absolute";
			canvas.absolutePositionning = 1;
		}
	},

	GetCanvasPositionX: function(thewindow)
	{
		var windowname=UTF8ToString(thewindow);
		var canvas =document.getElementById(windowname);
		
		//var canvas = Module['canvas'];
		
		return canvas.offsetLeft;
	},
	
	GetCanvasPositionY: function(thewindow)
	{
		var windowname=UTF8ToString(thewindow);
		var canvas =document.getElementById(windowname);

		//var canvas = Module['canvas'];
		return canvas.offsetTop;
	},
	
	SetCanvasSize:function (thewindow,a_width, a_height)
	{
		var windowname=UTF8ToString(thewindow);
		var canvas =document.getElementById(windowname);

		//var canvas = Module['canvas'];
		canvas.width = a_width;
		canvas.height = a_height;
	},
	
	SetCanvasPos:function (thewindow,posx,posy)
	{
		var windowname=UTF8ToString(thewindow);
		var canvas =document.getElementById(windowname);

        var isParentRelative = canvas.parentElement.position == "static";

		if(canvas.absolutePositionning) // we want an absolute position in the html doc
		{
			// test if position is parent relative
			if(isParentRelative)
			{
				var rect = canvas.parentElement.getBoundingClientRect();
				canvas.style.left = (rect.left + posx) +'px';
				canvas.style.top = (rect.top + posy) +'px';
			}
			else
			{
				canvas.style.left = posx +'px';
				canvas.style.top = posy +'px';
			}
		}
		else // want a relative position
		{
			if(isParentRelative)
			{
				canvas.style.left = posx +'px';
				canvas.style.top = posy +'px';
			}
			else
			{
				var rect = canvas.parentElement.getBoundingClientRect();
				canvas.style.left = (rect.left + posx) +'px';
				canvas.style.top = (rect.top + posy) +'px';
			}
		}
	},
	
	GetHTMLBodySizeX: function()
	{
		var width = window.innerWidth;
		return width;
	},
	
	GetHTMLBodySizeY: function()
	{
		var height = window.innerHeight;
		return height;
	},
	
	GetHTMLParentSizeX: function()
	{
		var width = Math.max(Module.canvas.parentElement.scrollWidth,Module.canvas.parentElement.clientWidth,Module.canvas.parentElement.offsetWidth);
		return width;
	},
	
	GetHTMLParentSizeY: function()
	{
		var height = Math.max(Module.canvas.parentElement.scrollHeight,Module.canvas.parentElement.clientHeight,Module.canvas.parentElement.offsetHeight);
		return height;
	},
	
	SetAbsolutePos: function(thewindow,abs)
	{
		var windowname=UTF8ToString(thewindow);
		var canvas =document.getElementById(windowname);
		canvas.absolutePositionning = abs;
	},
	
};

autoAddDeps(LibraryJsGUI, '$JsGUI');
mergeInto(LibraryManager.library, LibraryJsGUI);


