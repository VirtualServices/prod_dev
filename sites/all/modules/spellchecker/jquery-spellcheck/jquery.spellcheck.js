/*! Copyright (c) 2010 Brandon Aaron (http://brandonaaron.net)
 * Licensed under the MIT License (LICENSE.txt).
 *
 * Version 0.2
 */
(function($){

/*
 * Creates an instance of a SpellChecker for each matched element.
 * The SpellChecker has several configurable options.
 *  - lang: the 2 letter language code, defaults to en for english
 *  - events: a space separated string of events to use, default is 'keypress blur paste'
 *  - autocheck: number of milliseconds to check spelling after a key event, default is 750.
 *  - url: url of the spellcheck service on your server, default is spellcheck.php
 *  - ignorecaps: 1 to ignore words with all caps, 0 to check them
 *  - ignoredigits: 1 to ignore digits, 0 to check them
 */
$.fn.spellcheck = function(options) {
    return this.each(function() {
        var $this = $(this);
        if ( !$this.is('[type=password]') && !$(this).data('spellchecker') )
            $(this).data('spellchecker', new $.SpellChecker(this, options));
    });
};

/**
 * Forces a spell check on an element that has an instance of SpellChecker.
 */
$.fn.checkspelling = function() {
    return this.each(function() {
        var spellchecker = $(this).data('spellchecker');
        spellchecker && spellchecker.checkSpelling();
    });
};


$.SpellChecker = function(element, options) {
	this.ignored_words = new Array();
    this.$element = $(element);
    this.options = $.extend({
        lang: 'en',
        autocheck: 750,
        events: 'keypress blur paste',
        url: 'spellcheck.php',
        ignorecaps: 1,
        ignoredigits: 1
    }, options);
    this.bindEvents();
};

$.SpellChecker.prototype = {
		
    bindEvents: function() {
        if ( !this.options.events ) return;
        var self = this, timeout;
        this.$element.bind(this.options.events, function(event) {
            if ( /^key[press|up|down]/.test(event.type) ) {
                if ( timeout ) clearTimeout(timeout);
                timeout = setTimeout(function() { self.checkSpelling(); }, self.options.autocheck);
            } else
                self.checkSpelling();
            
        });
    },
    
    checkSpelling: function() {
        var prevText = this.text, text = this.$element.val(), self = this;
        if ( prevText === text ) return;
        this.text = this.$element.val();
        $.get(this.options.url, { 
            text: this.text, 
            lang: this.options.lang,
            ignorecaps: this.options.ignorecaps,
            ignoredigits: this.options.ignoredigits
        }, function(r, w) { self.parseResults(r); });
    },
    
    parseResults: function(results) {
        var self = this;
        this.results = [];
        $(results).find('c').each(function() {
            var $this = $(this),
                offset = $this.attr('o'),
                length = $this.attr('l');
            var tword = self.text.substr(offset, length);
            // Showing only non-ignored results
            if ($.inArray(tword, self.ignored_words) == -1) {
	            self.results.push({
	                word: tword,
	                suggestions: $this.text().split(/\s/)
	            });
            }
        });
        this.displayResults();
    },
    
    displayResults: function() {
    	
    	if($('#spellcheckresults').length) {
    		$('#spellcheckresults').remove();
    	}
    	
    	if($('#spellcheckresults-open-link').length) {
    		$('#spellcheckresults-open-link').remove();
    	}
    	
        if ( !this.results.length ) // @bro: applied patch from: http://plugins.jquery.com/node/9822
        {
          this.triggerComplete( true );
          return;
        }
        var $container = $('<div id="spellcheckresults"></div>').appendTo('body'),
            dl = [], self = this, offset = this.$element.offset(), height = this.$element[0].offsetHeight, i, k;
        
        var $closeLink = $('<a href="#" class="close-link">Close</a>');
	    $closeLink.bind('click', {'cnt': this.$element}, function(e) {
	    	
	    	var cnt = e.data.cnt;
	    	$container.hide();
	    	
		    var $openLink = $('<a href="#" id="spellcheckresults-open-link">Show Errors</a>');
		      $openLink.bind('click', function(e) {
		        $container.show();
		        $(this).remove();
		        e.preventDefault();
		     })
		     $openLink.insertAfter(cnt);
		      
	    	e.preventDefault();
	    });
	    $closeLink.prependTo($container);;

        for ( i=0; i<this.results.length; i++ ) {
            var result = this.results[i], suggestions = result.suggestions;
            dl.push('<dl><dt>'+result.word+'</dt>');
            for ( k=0; k<suggestions.length; k++ )
                dl.push('<dd class="word">'+suggestions[k]+'</dd>'); // @bro: added 'word' class
            dl.push('<dd class="ignore">ignore</dd><dd class="add">add</dd></dl>'); // @bro: added 'add' option
        }

        $container.append(dl.join('')).find('dd').bind('click', function(event) {
        	var $this = $(this), $parent = $this.parent(); 
            if ( $this.is('.word')) {
                self.$element.val( self.$element.val().replace( $parent.find('dt').text(), $this.text() ) );
            } else {
                self.triggerOption( $this.attr('class'), $parent.find('dt').text() );
            }
            $parent.remove();
            if (!$('#spellcheckresults dl').length) // @bro: applied patch from: http://plugins.jquery.com/node/9822
            {
              $('#spellcheckresults').remove();
              self.triggerComplete( false );
            }
            this.blur();
        }).end().css({ top: offset.top + height, left: offset.left });

    },

    triggerComplete: function( noneFound ) { // @bro: applied patch from: http://plugins.jquery.com/node/9822
      // Using xml-style namespace for custom events
      this.$element.trigger( 'spellchecker:complete', [noneFound] );
    },
    
    triggerOption: function( op, word ) { // @bro: trigger option
        switch (op) {
            case 'add':
                $.post("/spellchecker/" + op, { "word": word });
            break;
            case 'ignore':
            	// Storing list of ignored words
            	this.ignored_words.push(word);
            break;
            default:
        }
    }
    
};

})(jQuery);
