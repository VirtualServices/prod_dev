// Enable if necessary
// tinyMCE.onAddEditor.add(function(mgr,ed) {
  // console.debug('onAddEditor: ' + ed.id);
// });

/**
 * Attach this editor to a target element.
 *
 * See Drupal.wysiwyg.editor.attach.none() for a full desciption of this hook.
 */
Drupal.wysiwyg.editor.attach.tinymce = function(context, params, settings) {
  // Configure editor settings for this input format.
  var ed = new tinymce.Editor(params.field, settings);
  // Reset active instance id on any event.
  ed.onEvent.add(function(ed, e) {
    Drupal.wysiwyg.activeId = ed.id;
  });
  // Make toolbar buttons wrappable (required for IE).
  ed.onPostRender.add(function (ed) {
    var $toolbar = $('<div class="wysiwygToolbar"></div>');
    $('#' + ed.editorContainer + ' table.mceToolbar > tbody > tr > td').each(function () {
      $('<div></div>').addClass(this.className).append($(this).children()).appendTo($toolbar);
    });
    $('#' + ed.editorContainer + ' table.mceLayout td.mceToolbar').append($toolbar);
    $('#' + ed.editorContainer + ' table.mceToolbar').remove();
  });
  // Enable if necessary
  // ed.onPostRender.add(function(ed, cm) {
    // console.debug('onPostRender: ' + ed.id);
  // });
  ed.onLoadContent.add(function(ed, o) {
    // Enable if necessary
    // console.debug('onLoadContent: ' + ed.id);
    if (ed.execCommands['mceSpellCheck']) {
      ed.execCommand('mceSpellCheck');
    }
  });
  // Attach editor.
  ed.render();
};


Drupal.behaviors.spellchecker = function() {

    for (var form_id in Drupal.settings.spellchecker) {

        var form_settings = Drupal.settings.spellchecker[form_id];
        var jsonstr = form_settings.jsonstr;
        var timeout = form_settings.timeout;
        var lang = form_settings.lang;
        var basepath = form_settings.basepath;
        var ignorecaps = form_settings.ignorecaps;
        var ignoredigits = form_settings.ignoredigits;

        eval('var data = ' + jsonstr);

        var callback = function(index, item) {
            
            var field = $("textarea[name^='"+index+"'],input[name^='"+index+"'],textarea[id^='"+index+"'],input[id^='"+index+"']");

            $(field).spellcheck({ url: basepath + 'spellchecker', events: 'keyup', autocheck: timeout, lang: lang, ignorecaps: ignorecaps, ignoredigits: ignoredigits });
            
         };

         for ( name in data ) {
             if ( callback.call( this, name, data[ name ] ) === false ) break; 
         };

         $("#spellchecker").val("").remove();
         $("#spellchecker").val("").remove();
         $("#spellchecker_timeout").val("").remove();
         $("#spellchecker_timeout").val("").remove();
    };
};

function spellchecker_activate_spelling() {
  $.each(Drupal.wysiwyg.instances, function() {
    if (Drupal.wysiwyg.instances[this.field].editor == 'tinymce') {
        ed = tinyMCE.getInstanceById(Drupal.wysiwyg.instances[this.field].field);
        ed.execCommand('mceSpellCheck');
    }
  });
};

function spellchecker_result(data) {
    eval('var json_data = ' + data); 
    $.each(json_data, function(a,b) {
        eval(b);
    });
};

function spellchecker_post(field, fieldname, callback, action) {
    var formdata = $(field).parents("form:first").serialize();
    if (last_formdata != formdata) {
       $.post("/spellchecker/" + fieldname,{"data": formdata, "action": action, "callback": callback},spellchecker_result,"text");
       last_formdata = formdata;
    };
};


function spellchecker_prepost(field, fieldname, callback, action) {
    _field = field;
    _fieldname = fieldname;
    _callback = callback;
    _action = action;
    if (st > 0) clearTimeout(st);
    
    st = setTimeout('spellchecker_post(_field, _fieldname, _callback, _action)', timeout);
};

function spellchecker_post_confirm(title, teaser, form, tinymce) {
    var body = String(tinymce.selectedInstance.plugins.spellchecker._getWords());
    body = body.replace(/,/g, ' ');
    $.get("/spellchecker_confirm", { title: title, body: body, teaser: teaser, op: "invalid_list" }, 
        function(data) { 
            if(confirm(data)) {form.submit()}
        }
    ); 
};

