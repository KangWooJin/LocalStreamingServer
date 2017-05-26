HTTP / 1.1 200 OK
Cache - Control: no - cache
Server : libnhttpd
	 Date : Wed Jul 4 15 : 32 : 03 2012
		Connection : Keep - Alive :
					 Content - Type : application / rdf + xml
					 Content - Length : 24860
{
	"rowset":	{
		"osname":	"NCOMS",
			"dbname" : "alerts",
			"tblname" : "status",
			"coldesc" : [{
			"name":	"Identifier",
				"type" : "string",
				"size" : 255
		}, {
			"name": "Serial",
			"type" : "integer",
			"size" : 4
		}, {
			"name": "Node",
			"type" : "string",
			"size" : 64
		}, {
			"name": "NodeAlias",
			"type" : "string",
			"size" : 64
		}, {
			"name": "AlertKey",
			"type" : "string",
			"size" : 255
		}, {
			"name": "Severity",
			"type" : "integer",
			"size" : 4
		}, {
			"name": "Summary",
			"type" : "string",
			"size" : 255
		}, {
			"name": "StateChange",
			"type" : "utc",
			"size" : 4
		}, {
			"name": "FirstOccurrence",
			"type" : "utc",
			"size" : 4
		}, {
			"name": "LastOccurrence",
			"type" : "utc",
			"size" : 4
		}, {
			"name": "RowSerial",
			"type" : "integer",
			"size" : 4
		}],
		"rows":[{
				"Identifier": "Startup@sol9-build1",
					"Serial" : 12469,
					"Node" : "sol9-build1",
					"NodeAlias" : "",
					"AlertKey" : "",
					"Severity" : 0,
					"Summary" : "ObjectServer NCOMS on sol9-build1 started at Wed Jul 04 15:27:57 2012",
					"StateChange" : 1341412082,
					"FirstOccurrence" : 1341411978,
					"LastOccurrence" : 1341412077,
					"RowSerial" : 12469
			}, {
				"Identifier": "ProfilerEnableToggle@NCOMS:sol9-build1",
				"Serial" : 12468,
				"Node" : "sol9-build1",
				"NodeAlias" : "",
				"AlertKey" : "",
				"Severity" : 0,
				"Summary" : "ObjectServer NCOMS Profiler enabled at Wed Jul 04 15:27:56 2012",
				"StateChange" : 1341412077,
				"FirstOccurrence" : 1341411976,
				"LastOccurrence" : 1341412076,
				"RowSerial" : 12468
			}, {
				"Identifier": "JUnitEventTestInstance####0",
				"Serial" : 12469,
				"Node" : "sol9-build1",
				"NodeAlias" : "",
				"AlertKey" : "JUnitEventInstance",
				"Severity" : 0,
				"Summary" : "This is a test event generated by the JUnit REST Event Tests. (0)",
				"StateChange" : 1341412184,
				"FirstOccurrence" : 1341411772,
				"LastOccurrence" : 1341412074,
				"RowSerial" : 12468
			}, {
				"Identifier": "Shutdown@sol9-build1",
				******* TRUNCATED ********
				"RowSerial" : 12519
			}],
			"affectedRows": 12
	}
}