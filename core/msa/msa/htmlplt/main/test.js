function Main_Index() {

	var btn = new Ext.Button({
				text : '2323432432455565645654654',
				handler : function() {
					Ext.MessageBox.alert('1111', '2222222');
				}
			});
	var tabmainpanel = new Ext.Panel({
		    id: 'msamainpanel',
				items : [btn]
			});
	JsHelper.ExtTabDoLayout(tabmainpanel);// ע�������panel����ӵ���ǰ��tabpanel��
}
Main_Index();// ִ�з���
