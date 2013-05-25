Graph = function() {
	this.nodes = [];
	this.edges = [];
	this.state = undefined;
	this.mobile = 'ontouchstart' in window;
	this.drag = this.mobile ? [] : undefined;

	this.canvas = document.getElementsByTagName('canvas')[0];
	this.canvas.height = window.innerHeight;
	this.canvas.width = window.innerWidth;
	
	this.context = this.canvas.getContext('2d');
	
	var that = this;

	this.canvas.onselectstart = function () {
		return false;
	};
	this.canvas.onmousedown = function(e) {
		that.drag = that.hit(e);
	};
	this.canvas.ontouchstart = function(e) {
		e.preventDefault();
		for (var i = 0; i < e.changedTouches.length; i++) {
			var touch = e.changedTouches[i];
			var hit = that.hit(touch);

			if (hit)
			 	that.drag[touch.identifier] = hit;
		}
	};
	this.canvas.onmouseup = function(e) {
		that.drag = undefined;
	};
	this.canvas.ontouchend = function(e) {
		e.preventDefault();
		for (var i = 0; i < e.changedTouches.length; i++)
			delete that.drag[e.changedTouches[i].identifier];
	};
	this.canvas.onmousemove = function(e) {
		if (that.drag) {
			that.drag.node.position = {
				'x': e.clientX - that.drag.offset.x,
				'y': e.clientY - that.drag.offset.y
			};
			that.paint();
		}
	};
	this.canvas.ontouchmove = function(e) {
		e.preventDefault();
		for (var i = 0; i < e.changedTouches.length; i++) {
			var touch = e.changedTouches[i];
			var drag = that.drag[touch.identifier];
			if (drag) {
				drag.node.position = {
					'x': touch.clientX - drag.offset.x,
					'y': touch.clientY - drag.offset.y
				};
				that.paint();
			}
		}
	};
	
	window.onresize = function() {
		var d = {
			'x': window.innerWidth / that.canvas.width,
			'y': window.innerHeight / that.canvas.height
		};
		
		for(var i = 0; i < that.nodes.length; i++) {
			that.nodes[i].position.x *= d.x;
			that.nodes[i].position.y *= d.y;
			that.nodes[i].radius = Math.min(window.innerWidth, window.innerHeight) / 30;
		}

		that.canvas.height = window.innerHeight;
		that.canvas.width = window.innerWidth;
		that.paint();
	};
};

Graph.prototype.addNode = function(color, position, radius) {
	this.nodes.push({
		'color': color,
		'position': position,
		'radius': radius
	});
	this.paint();
};

Graph.prototype.addNodeRandom = function(count) {
	count = count || 1;
	
	var radius = Math.min(window.innerWidth, window.innerHeight) / 30;
	var border = radius * 2;

	for (var i = 0; i < count; i++)
		this.nodes.push({
			'color': '#' + ('000000' + (~~(Math.random() * 0xffffff)).toString(16)).slice(-6),
			'position': {
				'x': border + ~~(Math.random() * (window.innerWidth - border * 2)),
				'y': border + ~~(Math.random() * (window.innerHeight - border * 2))
			},
			'radius': radius
		});

	this.paint();
}

Graph.prototype.addEdge = function(a, b) {
	this.edges.push({
		'a': a,
		'b': b
	});
	this.paint();
};

Graph.prototype.addEdgeRandom = function(count) {
	count = count || 1;
	
	for (var i = 0; i < count; i++)
		this.edges.push({
			'a': this.nodes[~~(Math.random() * this.nodes.length)],
			'b': this.nodes[~~(Math.random() * this.nodes.length)]
		});
	
	this.paint();
}

Graph.prototype.paint = function(callback) {
	this.context.clearRect(0, 0, this.canvas.width, this.canvas.height);

	this.context.strokeStyle = 'rgba(0, 0, 0, 0.75)';
	this.context.save();

	for(var i = 0; i < this.edges.length; i++) {
		var edge = this.edges[i];
		var delta = Math.atan((edge.a.position.y - edge.b.position.y) / (edge.a.position.x - edge.b.position.x));	
		this.context.beginPath();
		this.context.moveTo(edge.a.position.x + Math.cos(delta) * edge.a.radius, edge.a.position.y + Math.sin(delta) * edge.a.radius);
		this.context.lineTo(edge.b.position.x + Math.cos(delta) * edge.b.radius, edge.b.position.y + Math.sin(delta) * edge.b.radius);
		this.context.closePath();
		this.context.stroke();
	}

	for(var i = 0; i < this.nodes.length; i++) {
		var node = this.nodes[i];
		this.context.fillStyle = node.color;
		this.context.beginPath();
		this.context.arc(node.position.x, node.position.y, node.radius, 0, 2 * Math.PI, false);
		this.context.closePath();
		this.context.stroke();
		this.context.fill();
	}
	
	this.context.restore();
		
	if (callback)
		callback();
};

Graph.prototype.hit = function(e) {
	for(var i = this.nodes.length - 1; i >= 0; i--)
		if (Math.pow(e.clientX - this.nodes[i].position.x, 2) + Math.pow(e.clientY - this.nodes[i].position.y, 2) < Math.pow(this.nodes[i].radius, 2))
			return {
				'node': this.nodes[i],
				'offset': {
					'x': e.clientX - this.nodes[i].position.x,
					'y': e.clientY - this.nodes[i].position.y
				}
			};

	return undefined;
};
