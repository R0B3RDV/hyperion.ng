
function bindNavToContent(containerId, fileName, loadNow)
{
	$("#page-content").off();
	$(containerId).on("click", function() {
		$("#page-content").load("/content/"+fileName+".html");
	});
	if (loadNow)
	{
		$(containerId).trigger("click");
	}
}

function loadContentTo(containerId, fileName)
{
	$(containerId).load("/content/"+fileName+".html");
}



function toggleClass(obj,class1,class2)
{
	if ( $(obj).hasClass(class1))
	{
		$(obj).removeClass(class1);
		$(obj).addClass(class2);
	}
	else
	{
		$(obj).removeClass(class2);
		$(obj).addClass(class1);
	}
}


function setClassByBool(obj,enable,class1,class2)
{
	if (enable)
	{
		$(obj).removeClass(class1);
		$(obj).addClass(class2);
	}
	else
	{
		$(obj).removeClass(class2);
		$(obj).addClass(class1);
	}
}

function showErrorDialog(header,message)
{
	$('#error_dialog .modal-title').html(header);
	$('#error_dialog .modal-body').html(message);
	$('#error_dialog').modal('show');
}

function isJsonString(str) {
	try {
		JSON.parse(str);
	} catch (e) {
		return e;
	}
	return "";
}
