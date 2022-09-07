export namespace main {
	
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

