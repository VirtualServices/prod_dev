$(document).ready(function() {

var url = $.url(); // parse the current page URL
var url_seg_one = (url.segment(1));
var url_seg_two = (url.segment(2));

//get the url of the page
var str = window.location;

// Replace url segment so the code works from the many urls it may be accessed from including
// dev and staging

if(url_seg_one == "summer"){
   $('.src_adults_info').hide(2000);
   $('.src_teens_info').hide(2000);
   $('.src_children_info').hide(2000);
   $('.src_read2me_info').hide(2000);
}

$('.src_adults_arrow').click(function() {
  $('.src_adults_info').toggle('fast', function() {
    // Animation complete.
  });
});

$('.src_adults').click(function() {
  $('.src_adults_info').toggle('fast', function() {
    // Animation complete.
  });
});

$('.src_teens_arrow').click(function() {
  $('.src_teens_info').toggle('fast', function() {
    // Animation complete.
  });
});

$('.src_teens').click(function() {
  $('.src_teens_info').toggle('fast', function() {
    // Animation complete.
  });
});

$('.src_children_arrow').click(function() {
  $('.src_children_info').toggle('fast', function() {
    // Animation complete.
  });
});

$('.src_children').click(function() {
  $('.src_children_info').toggle('fast', function() {
    // Animation complete.
  });
});

$('.src_read2me_arrow').click(function() {
  $('.src_read2me_info').toggle('fast', function() {
    // Animation complete.
  });
});

$('.src_read2me').click(function() {
  $('.src_read2me_info').toggle('fast', function() {
    // Animation complete.
  });
});

if(url_seg_one == "summer" && url_seg_two == "weekly-challenge"){
   $('.week1_info').hide(2000);
   $('.week2_info').hide(2000);
   $('.week3_info').hide(2000);
   $('.week4_info').hide(2000);
   $('.week5_info').hide(2000);
   $('.week6_info').hide(2000);
   $('.week7_info').hide(2000);
   $('.week8_info').hide(2000);
   $('.week9_info').hide(2000);
   $('.week10_info').hide(2000);
   }
   

   
$('.week1_arrow').click(function() {
  $('.week1_info').toggle('fast', function() {
    // Animation complete.
  });
});

$('.week1').click(function() {
  $('.week1_info').toggle('fast', function() {
    // Animation complete.
  });
});

$('.week2_arrow').click(function() {
  $('.week2_info').toggle('fast', function() {
    // Animation complete.
  });
});

$('.week2').click(function() {
  $('.week2_info').toggle('fast', function() {
    // Animation complete.
  });
});

$('.week3_arrow').click(function() {
  $('.week3_info').toggle('fast', function() {
    // Animation complete.
  });
});

$('.week3').click(function() {
  $('.week3_info').toggle('fast', function() {
    // Animation complete.
  });
});

$('.week4_arrow').click(function() {
  $('.week4_info').toggle('fast', function() {
    // Animation complete.
  });
});

$('.week4').click(function() {
  $('.week4_info').toggle('fast', function() {
    // Animation complete.
  });
});

$('.week5_arrow').click(function() {
  $('.week5_info').toggle('fast', function() {
    // Animation complete.
  });
});

$('.week5').click(function() {
  $('.week5_info').toggle('fast', function() {
    // Animation complete.
  });
});

$('.week6_arrow').click(function() {
  $('.week6_info').toggle('fast', function() {
    // Animation complete.
  });
});

$('.week6').click(function() {
  $('.week6_info').toggle('fast', function() {
    // Animation complete.
  });
});

$('.week7_arrow').click(function() {
  $('.week7_info').toggle('fast', function() {
    // Animation complete.
  });
});

$('.week7').click(function() {
  $('.week7_info').toggle('fast', function() {
    // Animation complete.
  });
});

$('.week8_arrow').click(function() {
  $('.week8_info').toggle('fast', function() {
    // Animation complete.
  });
});

$('.week8').click(function() {
  $('.week8_info').toggle('fast', function() {
    // Animation complete.
  });
});

$('.week9_arrow').click(function() {
  $('.week9_info').toggle('fast', function() {
    // Animation complete.
  });
});

$('.week9').click(function() {
  $('.week9_info').toggle('fast', function() {
    // Animation complete.
  });
});

$('.week10_arrow').click(function() {
  $('.week10_info').toggle('fast', function() {
    // Animation complete.
  });
});

$('.week10').click(function() {
  $('.week10_info').toggle('fast', function() {
    // Animation complete.
  });
});

//get the url of the page
var str = window.location;

if(url_seg_one == "downloadables"){
   $('.ebooks').hide(2000);
   $('.audiobooks').hide(2000);
   $('.music').hide(2000);
   $('.video').hide(2000);
}


   
$('.ebooks_arrow').click(function() {
  $('.ebooks').toggle('fast', function() {
    // Animation complete.
  });
});

$('.ebooks_title').click(function() {
  $('.ebooks').toggle('fast', function() {
    // Animation complete.
  });
});

$('.audiobooks_arrow').click(function() {
  $('.audiobooks').toggle('fast', function() {
    // Animation complete.
  });
});

$('.audiobooks_title').click(function() {
  $('.audiobooks').toggle('fast', function() {
    // Animation complete.
  });
});

$('.music_arrow').click(function() {
  $('.music').toggle('fast', function() {
    // Animation complete.
  });
});

$('.music_title').click(function() {
  $('.music').toggle('fast', function() {
    // Animation complete.
  });
});

$('.video_arrow').click(function() {
	  $('.video').toggle('fast', function() {
	    // Animation complete.
	  });
	});

	$('.video_title').click(function() {
	  $('.video').toggle('fast', function() {
	    // Animation complete.
	  });
	});


});

