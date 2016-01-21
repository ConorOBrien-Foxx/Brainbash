window.addEventListener("load",function(){
	document.getElementById("submit").addEventListener("click",function(){
		var inp = document.getElementById("code");
		window.bb = new Brainbash(inp.value);
		bb.display();
		document.getElementById("output").innerHTML = "";
	});
	document.getElementById("run").addEventListener("click",function(){
		function run(){
			var res = bb.step();
			bb.display();
			if(res) setTimeout(run,(+document.getElementById("timeout").value)||20);
		}
		run();
	});
	document.getElementById("run2").addEventListener("click",function(){
		while(bb.step())bb.display();
	});
	document.getElementById("step").addEventListener("click",function(){
		bb.step();
		bb.display();
	});
});

function Brainbash(code,max,min){
	this.code   = code;
	this.index  = 0;
	this.ptrA   = 0;
	this.tapeA  = [0];
	this.ptrB   = 0;
	this.tapeB  = [0];
	this.curSym = "A";
	this.maxSze = typeof max==="undefined"?255:max;
	this.minSze = typeof min==="undefined"?0:min;
	this.macros = "";
	this.mode   = 0;
	this.finMac = {};
}

Brainbash.prototype.step = function(){
	if(this.index>=this.code.length) return false;
	var chr = this.code[this.index];
	if(this.mode==0){
		if(typeof this.finMac[chr]!=="undefined"){
			this.code = this.code.slice(0,this.index-1)+this.finMac[chr]+this.code.slice(this.index+1,this.code.length);
			this.index-=2;
		} else {
			switch(chr){
				case "+":
					this["tape"+this.curSym][this["ptr"+this.curSym]]++;
					break;
				case "-":
					this["tape"+this.curSym][this["ptr"+this.curSym]]--;
					break;
				case ">":
					this["ptr"+this.curSym]++;
					break;
				case "<":
					this["ptr"+this.curSym]--;
					break;
				case ".":
					document.getElementById("output").innerHTML += String.fromCharCode(this["tape"+this.curSym][this["ptr"+this.curSym]]);
					break;
				case "~":
					this.curSym = "AB"[Number(this.curSym=="A")];
					break;
				case "{":
					var val = this["tape"+this.curSym][this["ptr"+this.curSym]];
					for(var i=0;i<Math.abs(val);i++){
						this["ptr"+this.curSym]--;
						this["tape"+this.curSym][this["ptr"+this.curSym]] = this["tape"+this.curSym][this["ptr"+this.curSym]]||0;
					}
					break;
				case "}":
					var val = this["tape"+this.curSym][this["ptr"+this.curSym]];
					for(var i=0;i<Math.abs(val);i++){
						this["ptr"+this.curSym]++;
						this["tape"+this.curSym][this["ptr"+this.curSym]] = this["tape"+this.curSym][this["ptr"+this.curSym]]||0;
					}
					break;
				case "@":
					this["ptr"+this.curSym] = this["tape"+this.curSym][this["ptr"+this.curSym]];
					for(var i=0;i<this["ptr"+this.curSym];i++){
						this["tape"+this.curSym][i] = this["tape"+this.curSym][i]||0;
					}
					break;
				case ":":
					this.mode = 1;
					break;
				case "]":
					if(this["tape"+this.curSym][this["ptr"+this.curSym]]){
						// find previous bracket
						var depth = 1;
						for(var j=this.index;--j&&depth;){
							if(this.code[j]=="]") depth++;
							if(this.code[j]=="[") depth--;
						}
						this.index = j;
					}
					break;
				case "*":
					var ind = this["ptr"+this.curSym];
					this.curSym = "AB"[Number(this.curSym=="A")];
					this["ptr"+this.curSym] = ind;
					for(var i=0;i<ind;i++){
						this["tape"+this.curSym][i] = this["tape"+this.curSym][i]||0;
					}
					break;
				case "x":
					this["tape"+this.curSym][this["ptr"+this.curSym]] = 0;
					break;
			}
		}
		this["tape"+this.curSym][this["ptr"+this.curSym]] = this["tape"+this.curSym][this["ptr"+this.curSym]]||0;
		if(this["tape"+this.curSym][this["ptr"+this.curSym]]<this.minSze) this["tape"+this.curSym][this["ptr"+this.curSym]]+=this.maxSize+1;
	} else if(this.mode==1){
		console.log(chr);
		if(chr==":"){this.mode = 0;this.finMac[this.code[++this.index]] = this.macros; this.macros = ""; }
		else this.macros += chr;
	}
	if(++this.index<this.code.length) return this;
	return false;
}

Brainbash.prototype.display = function(){
	var l = Math.max(this.tapeA.length,this.tapeB.length);
	var a = "<table class=\"code\"><tr><td class=\"long\">Code: </td></td>";
	for(var i=0;i<=this.code.length;i++){
		a+="<td"+(i==this.index?" class=\"cur\"":"")+">"+(typeof this.code[i]==="undefined"?"EOF":this.code[i])+"</td>";
	}
	a+="</tr></table>";
	var x = "<table class=\"code\">"
	for(var j=0;j<2;j++){
		x+="<tr>";
		var y="<tr>";
		for(var i=0;i<l;i++){
			var sym = this["tape"+"AB"[j]][i];
			x+="<td>"+(typeof sym==="undefined"?"":sym)+"</td>";
			y+="<td>"+("AB"[j]==this.curSym?"<em>":"")+(i==this["ptr"+"AB"[j]]?"^":"")+("AB"[j]==this.curSym?"</em>":"")+"</td>";
		}
		x+="</tr>"+y+"</tr>";
	}
	document.getElementById("action").innerHTML = a+x;
	return "["+this.tapeA.join(" ")+"]\n["+this.tapeB.join(" ")+"]";
}
/*
+++[[->+~+*]>-~[-*+*]>~>]
<<
[[>[-~+~>+<]>[-<+>]<<-]~[-~+~]~<]
*/
