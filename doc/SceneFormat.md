Scene format (draft)
=====================

The engine will be able to load and save scenes for further reuse.
At the moment the save format uses JSON as a transport layer,
but the structure might remain the same for other formats.

Like any other JSON file, the data is encapsulated into an object containing
serialization metadata and a field containing the actual data.

Fields starting with an @ are file-only.

@serial : which format is used in the JSON file
@id : pointer emulation to identify an object inside the file


Example
------------

	{
		"@serial":"SNS1",
		"data":{
			"children":[
				{
					"type":"GameManager"
				},
				{
					"type":"Entity",
					"properties":{
						"name":"dude",
					}
					"children":[
						{
							"@id":"1111111111111111"
							"type":"Sprite",
							"properties":{
								"name":"background",
								"texture":"razor_drone",
								"frame":0,
							}
						},
						{
							"@id":"2222222222222222"
							"type":"Sprite",
							"properties":{
								"name":"foreground",
								"texture":"razor_drone",
								"frame":1,
							}
						},
						{
							"@id":"3333333333333333"
							"type":"CollisionShape",
							"properties":{
								"name":"shoot_hit_box",
								"shape":{
									"points":[
										[0,0],
										[1,0],
										[1,1],
										[0,1]
									]
								}
							}
						}
					]
				}
			]
		}
	}

