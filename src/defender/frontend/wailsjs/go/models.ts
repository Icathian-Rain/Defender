export namespace main {
	
	export class Config {
	    syringePath: string;
	    dllPath: string;
	
	    static createFrom(source: any = {}) {
	        return new Config(source);
	    }
	
	    constructor(source: any = {}) {
	        if ('string' === typeof source) source = JSON.parse(source);
	        this.syringePath = source["syringePath"];
	        this.dllPath = source["dllPath"];
	    }
	}
	export class Result {
	    type: string;
	    title: string;
	    err: string;
	
	    static createFrom(source: any = {}) {
	        return new Result(source);
	    }
	
	    constructor(source: any = {}) {
	        if ('string' === typeof source) source = JSON.parse(source);
	        this.type = source["type"];
	        this.title = source["title"];
	        this.err = source["err"];
	    }
	}
	export class Msg {
	    funcName: string;
	    time: string;
	    info: {[key: string]: string};
	    result: Result;
	
	    static createFrom(source: any = {}) {
	        return new Msg(source);
	    }
	
	    constructor(source: any = {}) {
	        if ('string' === typeof source) source = JSON.parse(source);
	        this.funcName = source["funcName"];
	        this.time = source["time"];
	        this.info = source["info"];
	        this.result = this.convertValues(source["result"], Result);
	    }
	
		convertValues(a: any, classs: any, asMap: boolean = false): any {
		    if (!a) {
		        return a;
		    }
		    if (a.slice) {
		        return (a as any[]).map(elem => this.convertValues(elem, classs));
		    } else if ("object" === typeof a) {
		        if (asMap) {
		            for (const key of Object.keys(a)) {
		                a[key] = new classs(a[key]);
		            }
		            return a;
		        }
		        return new classs(a);
		    }
		    return a;
		}
	}
	
	export class Statistics {
	    totalNum: number;
	    successNum: number;
	    warningNum: number;
	    errorNum: number;
	    messageboxNum: number;
	    fileNum: number;
	    heapNum: number;
	    regNum: number;
	    netNum: number;
	    memoryNum: number;
	
	    static createFrom(source: any = {}) {
	        return new Statistics(source);
	    }
	
	    constructor(source: any = {}) {
	        if ('string' === typeof source) source = JSON.parse(source);
	        this.totalNum = source["totalNum"];
	        this.successNum = source["successNum"];
	        this.warningNum = source["warningNum"];
	        this.errorNum = source["errorNum"];
	        this.messageboxNum = source["messageboxNum"];
	        this.fileNum = source["fileNum"];
	        this.heapNum = source["heapNum"];
	        this.regNum = source["regNum"];
	        this.netNum = source["netNum"];
	        this.memoryNum = source["memoryNum"];
	    }
	}
	export class ProcessInfo {
	    processName: string;
	    processID: number;
	    processDll: string[];
	    processPriority: string;
	
	    static createFrom(source: any = {}) {
	        return new ProcessInfo(source);
	    }
	
	    constructor(source: any = {}) {
	        if ('string' === typeof source) source = JSON.parse(source);
	        this.processName = source["processName"];
	        this.processID = source["processID"];
	        this.processDll = source["processDll"];
	        this.processPriority = source["processPriority"];
	    }
	}

}

