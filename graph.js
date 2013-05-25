Graph = function(fullscreen) {
	this.nodes = [];
	this.edges = [];
	this.clusters = [];
	this.state = undefined;
	this.mobile = 'ontouchstart' in window;
	this.drag = this.mobile ? [] : undefined;

	this.canvas = document.getElementsByTagName('canvas')[0];

	if (fullscreen) {
		this.canvas.height = window.innerHeight;
		this.canvas.width = window.innerWidth;
	}
	
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
		that.dragging(e, that.drag);
	};
	this.canvas.ontouchmove = function(e) {
		e.preventDefault();
		for (var i = 0; i < e.changedTouches.length; i++)
			that.dragging(e.changedTouches[i], that.drag[e.changedTouches[i].identifier]);
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
		for (var i = 0; i < that.clusters.length; i++) {
			that.clusters[i].position.x *= d.x;
			that.clusters[i].position.y *= d.y;
			that.clusters[i].size.x *= d.x;
			that.clusters[i].size.y *= d.y;
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
		'radius': radius,
		'cluster': 0
	});
	this.paint();
};

Graph.prototype.addNodeRandom = function(count, cluster) {
	count = count || 1;
	cluster = this.clusters[cluster] || this.clusters[0];
	
	var radius = Math.min(window.innerWidth, window.innerHeight) / 30;
	var border = radius * 2;

	for (var i = 0; i < count; i++)
		this.nodes.push({
			'color': '#' + ('000000' + (~~(Math.random() * 0xffffff)).toString(16)).slice(-6),
			'position': {
				'x': cluster.position.x + border + ~~(Math.random() * (cluster.size.x - border * 2)),
				'y': cluster.position.y + border + ~~(Math.random() * (cluster.size.y - border * 2))
			},
			'radius': radius,
			'cluster': cluster
		});

	this.paint();
}

Graph.prototype.addEdge = function(a, b) {
	if (b < a) {
		var tmp = a;
		a = b;
		b = a;
	}

	this.edges.push({
		'a': this.nodes[a],
		'b': this.nodes[b]
	});
	this.paint();
};

Graph.prototype.addEdgeRandom = function(count) {
	count = count || 1;

	for (var i = 0; i < count; i++) {
		var a = ~~(Math.random() * this.nodes.length);
		var b = ~~(Math.random() * this.nodes.length);

		while (b == a)
			b = ~~(Math.random() * this.nodes.length);

		if (b < a) {
			var tmp = a;
			a = b;
			b = a;
		}

		a = this.nodes[a];
		b = this.nodes[b];

		var ok = true;
		for (var j = 0; j < this.edges.length; j++) {
			var edge = this.edges[j];
			if (edge.a == a && edge.b == b) {
				ok = false;
				break;
			}
		}

		if (!ok) {
			i--;
			continue;
		}
	
		this.edges.push({
			'a': a,
			'b': b
		});
	}

	this.paint();
}

Graph.prototype.addCluster = function(position, size) {
	position.x = (position.x / 100) * window.innerWidth;
	position.y = (position.y / 100) * window.innerHeight;
	size.x = (size.x / 100) * window.innerWidth;
	size.y = (size.y / 100) * window.innerHeight;
	this.clusters.push({'position': position, 'size': size, 'radius': Math.min(window.innerWidth, window.innerHeight) / 30});
}

Graph.prototype.paint = function(callback) {
	this.context.clearRect(0, 0, this.canvas.width, this.canvas.height);

	this.context.strokeStyle = 'rgba(0, 0, 0, 0.45)';
	this.context.lineWidth = 2;

	for(var i = 0; i < this.edges.length; i++) {
		var edge = this.edges[i];
		var delta = Math.atan((edge.a.position.y - edge.b.position.y) / (edge.a.position.x - edge.b.position.x));	
		this.context.beginPath();
		this.context.moveTo(edge.a.position.x + Math.cos(delta) * edge.a.radius, edge.a.position.y + Math.sin(delta) * edge.a.radius);
		this.context.lineTo(edge.b.position.x + Math.cos(delta) * edge.b.radius, edge.b.position.y + Math.sin(delta) * edge.b.radius);
		this.context.closePath();
		this.context.stroke();
	}

	this.context.strokeStyle = 'black';

	for(var i = 0; i < this.nodes.length; i++) {
		var node = this.nodes[i];
		this.context.fillStyle = node.color;
		this.context.beginPath();
		this.context.arc(node.position.x, node.position.y, node.radius, 0, 2 * Math.PI, false);
		this.context.closePath();
		this.context.stroke();
		this.context.fill();
	}

	this.context.strokeStyle = '#333';

	for (var i = 0; i < this.clusters.length; i++) {
		var cluster = this.clusters[i];

		this.context.beginPath();
		this.context.moveTo(cluster.position.x + cluster.radius, cluster.position.y);
		this.context.lineTo(cluster.position.x + cluster.size.x - cluster.radius, cluster.position.y);
		this.context.quadraticCurveTo(cluster.position.x + cluster.size.x, cluster.position.y, cluster.position.x + cluster.size.x, cluster.position.y + cluster.radius);
		this.context.lineTo(cluster.position.x + cluster.size.x, cluster.position.y + cluster.size.y - cluster.radius);
		this.context.quadraticCurveTo(cluster.position.x + cluster.size.x, cluster.position.y + cluster.size.y, cluster.position.x + cluster.size.x - cluster.radius, cluster.position.y + cluster.size.y);
		this.context.lineTo(cluster.position.x + cluster.radius, cluster.position.y + cluster.size.y);
		this.context.quadraticCurveTo(cluster.position.x, cluster.position.y + cluster.size.y, cluster.position.x, cluster.position.y + cluster.size.y - cluster.radius);
		this.context.lineTo(cluster.position.x, cluster.position.y + cluster.radius);
		this.context.quadraticCurveTo(cluster.position.x, cluster.position.y, cluster.position.x + cluster.radius, cluster.position.y);
		this.context.closePath();
		this.context.stroke();
	}

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

Graph.prototype.dragging = function(e, drag) {
	if (!drag)
		return;

	var cluster = drag.node.cluster;
	var position = {
		'x': e.clientX - drag.offset.x,
		'y': e.clientY - drag.offset.y
	};

	var ok = {
		'x':
				(position.x - drag.node.radius > cluster.position.x)
			&&
				(position.x + drag.node.radius < cluster.position.x + cluster.size.x),
		'y':
				(position.y - drag.node.radius > cluster.position.y)
			&&
				(position.y + drag.node.radius < cluster.position.y + cluster.size.y)
	};

	if (ok.x || ok.y) {
		if (ok.x) {
			drag.node.position.x = position.x;
		}
		if (ok.y) {
			drag.node.position.y = position.y;
		}
		this.paint();
	}
}
