#format SML1
{
	entities:[
		{
			@type:"sn::render::Camera"
			clearBits:["color", "depth"]
			clearColor: [0.5, 0.5, 0.8]
		},
		{
			@type:"tgui::GUI"
			theme:"example_theme"
			name:"the_gui"

			_children:[
				{
					@type:"tgui::Renderer"
				},
				{
					@type:"tgui::Panel"
					name:"lolpanel"
					bounds:{x:10, y:10, w:300, h:200}
					padding:{left:4, right:4, top:4, bottom:4}
					isResizeable:true

					_children:[
						{
							@type:"tgui::Button"
							bounds:{x:10,y:10,w:100,h:24}
							_children:[
								{
									@type:"tgui::Text"
									text:"Hello world!"
									bounds:{x:12, y:17, w:100, h:24}
									anchors:["left", "right", "top", "bottom"]
								}
							]
						},
						{
							@type:"tgui::Button"
							name:"scripted_button"
							bounds:{x:10,y:50,w:200,h:50}
							anchors:["left", "right"]
							script:{
								class:"ButtonScript"
							}
							_children:[
								{
									@type:"tgui::Text"
									text:"Click me, I'm a button\nWith a lot of text in it, blah blah blah...\nMultiple lines of text!"
									align:"center"
									anchors:["left", "right", "top", "bottom"]
								}
							]
						},
						{
							@type:"tgui::Slider"
							bounds:{x:10, y:110, w:200, h:20}
						},
						{
							@type:"tgui::Slider"
							bounds:{x:10, y:130, w:200, h:20}
						},
						{
							@type:"tgui::Slider"
							bounds:{x:10, y:150, w:200, h:20}
						},
						{
							@type:"tgui::TextArea"
							anchors:["left", "right", "bottom"]
							text:"This is not a sample text\nAnother line\nLorem ipsum dolor sit amet, consectetur adipiscing elit.\nThis is not a sample text\nAnother line\nLorem ipsum dolor sit amet, consectetur adipiscing elit."
						}
					]
				}
			]
		}
	]
}

