Graph = function(fullscreen) {
	this.nodes = [];
	this.edges = [];
	this.clusters = [];
	this.state = undefined;
	this.mobile = 'ontouchstart' in window;
	this.drag = this.mobile ? [] : undefined;
	this.selectedColor = undefined;
	this.conflicts = 0;
	this.neutralColor = 'rgb(150, 150, 150)';
	this.heightFactor = 0.95;

	this.canvas = document.getElementsByTagName('canvas')[0];

	if (fullscreen) {
		this.canvas.height = window.innerHeight * this.heightFactor;
		this.canvas.width = window.innerWidth;
	}
	
	this.context = this.canvas.getContext('2d');
	
	var that = this;

	this.canvas.onselectstart = function () {
		return false;
	};
	this.canvas.onmousedown = function(e) {
		if (that.selectedColor)
			return;

		that.drag = that.hit(e);
		if (that.drag)
			that.paint();
	};
	this.canvas.ontouchstart = function(e) {
		e.preventDefault();
		if (!that.drag)
			return;

		for (var i = 0; i < e.changedTouches.length; i++) {
			var touch = e.changedTouches[i];
			var hit = that.hit(touch);

			if (hit) {
				that.drag[touch.identifier] = hit;

				if (that.selectedColor) {
					for (var j = 0; j < that.nodes.length; j++) {
						if (that.nodes[j].cluster == hit.node.cluster)
							that.nodes[j].color = that.neutralColor;
					}
					hit.node.color = that.selectedColor;
					that.selectedColor = undefined;
					that.paint();
					that.check();
				}
				else {
					that.paint();
				}
			}
		}
	};
	this.canvas.onmouseup = function(e) {
		that.drag = undefined;
	};
	this.canvas.ontouchend = function(e) {
		e.preventDefault();
		if (e.changedTouches && that.drag)
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
	this.canvas.onclick = function(e) {
		var hit = that.hit(e);
		if (hit && that.selectedColor) {
			for (var i = 0; i < that.nodes.length; i++) {
				if (that.nodes[i].cluster == hit.node.cluster)
					that.nodes[i].color = that.neutralColor;
			}
			hit.node.color = that.selectedColor;
			that.selectedColor = undefined;
			that.paint();
			that.check();
		}
	};
	
/*	window.onresize = function() {
		var d = {
			'x': window.innerWidth / that.canvas.width,
			'y': (window.innerHeight * that.heightFactor) / that.canvas.height
		};
		//that.canvas.height = window.innerHeight * that.heightFactor;
		//that.canvas.width = window.innerWidth;

		for(var i = 0; i < that.nodes.length; i++) {
			that.nodes[i].position.x *= d.x;
			that.nodes[i].position.y *= d.y;
			that.nodes[i].radius = Math.min(that.canvas.width, that.canvas.height) / 30;
		}
		for (var i = 0; i < that.clusters.length; i++) {
			that.clusters[i].position.x *= d.x;
			that.clusters[i].position.y *= d.y;
			that.clusters[i].size.x *= d.x;
			that.clusters[i].size.y *= d.y;
		}
		that.paint();
	};*/
};

Graph.prototype.addNode = function(color, position, radius) {
	this.nodes.push({
		'color': that.neutralColor,
		'position': position,
		'radius': radius,
		'cluster': 0
	});
	this.paint();
};

Graph.prototype.addNodeRandom = function(count, cluster) {
	count = count || 1;
	cluster = this.clusters[cluster] || this.clusters[0];
	
	var radius = Math.min(this.canvas.width, this.canvas.height) / 30;
	var border = radius * 2;

	for (var i = 0; i < count; i++)
		this.nodes.push({
			'color': this.neutralColor,
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
		var a = ~~(Math.random() * this.nodes.length), b;

		while ((b = ~~(Math.random() * this.nodes.length)) == a);

		if (b < a) {
			var tmp = a;
			a = b;
			b = tmp;
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
	position.x = (position.x / 100) * this.canvas.width;
	position.y = (position.y / 100) * this.canvas.height;
	size.x = (size.x / 100) * this.canvas.width;
	size.y = (size.y / 100) * this.canvas.height;
	this.clusters.push({'position': position, 'size': size, 'radius': Math.min(this.canvas.width, this.canvas.height) / 30});
}

Graph.prototype.paint = function(callback) {
	this.context.clearRect(0, 0, this.canvas.width, this.canvas.height);

	this.conflicts = 0;
	this.colored = 0;

	for(var i = 0; i < this.edges.length; i++) {
		var edge = this.edges[i];
		var delta = Math.atan((edge.a.position.y - edge.b.position.y) / (edge.a.position.x - edge.b.position.x));
		var start = {'x': edge.a.position.x + Math.cos(delta) * edge.a.radius, 'y': edge.a.position.y + Math.sin(delta) * edge.a.radius};
		var stop = {'x': edge.b.position.x + Math.cos(delta) * edge.b.radius, 'y': edge.b.position.y + Math.sin(delta) * edge.b.radius};

		if (edge.a.color == edge.b.color && edge.a.color != this.neutralColor) {
			this.context.strokeStyle = 'rgba(255, 50, 50, 0.9)';
			this.context.lineWidth = 4;
			this.conflicts++;
		}
		else if (edge.a.color != this.neutralColor || edge.b.color != this.neutralColor) {
			this.context.lineWidth = 3;

			var gradient = this.context.createLinearGradient(start.x, start.y, stop.x, stop.y);
			gradient.addColorStop(0, edge.a.color);
			gradient.addColorStop(1, edge.b.color);
			this.context.strokeStyle = gradient;
		}
		else {
			this.context.strokeStyle = 'rgba(0, 0, 0, 0.45)';
			this.context.lineWidth = 2;
		}
	
		this.context.beginPath();
		this.context.moveTo(start.x, start.y);
		this.context.lineTo(stop.x, stop.y);
		this.context.closePath();
		this.context.stroke();
	}

	this.context.strokeStyle = 'black';
	this.context.lineWidth = 2;

	for(var i = 0; i < this.nodes.length; i++) {
		var node = this.nodes[i];

		if (node.color != this.neutralColor)
			this.colored++;

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
		if (Math.pow((0-this.canvas.offsetLeft) + e.clientX - this.nodes[i].position.x, 2) + Math.pow((0-this.canvas.offsetTop) + e.clientY - this.nodes[i].position.y, 2) < Math.pow(this.nodes[i].radius, 2)) {
			var node = this.nodes[i];
			this.nodes.splice(this.nodes.length - 1, 0, this.nodes.splice(i, 1)[0]);
			return {
				'node': node,
				'offset': {
					'x': e.clientX - node.position.x,
					'y': e.clientY - node.position.y
				}
			};
		}

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

Graph.prototype.selectColor = function(color) {
	if (this.neutralColor == color) {
		console.log('Tried to select neutral color!');
	}
	else {
		this.selectedColor = color;
	}
}

Graph.prototype.check = function() {
	if (this.conflicts == 0 && this.colored == this.clusters.length)
		alert('You solved this instance!');
}
