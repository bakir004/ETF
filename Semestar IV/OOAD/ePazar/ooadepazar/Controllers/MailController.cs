using Microsoft.AspNetCore.Mvc;
using System.Net;
using System.Net.Mail;

namespace ooadepazar.Controllers
{
    [ApiController]
    [Route("[controller]")]
    public class MailController : ControllerBase
    {
        private readonly IConfiguration _config;

        public MailController(IConfiguration config)
        {
            _config = config;
        }

        [HttpPost("Send")]
        public async Task<IActionResult> SendMail([FromBody] MailRequest request)
        {
            var smtpClient = new SmtpClient(_config["Smtp:Host"])
            {
                Port = int.Parse(_config["Smtp:Port"]),
                Credentials = new NetworkCredential(_config["Smtp:Username"], _config["Smtp:Password"]),
                EnableSsl = true
            };

            var mail = new MailMessage
            {
                From = new MailAddress(_config["Smtp:Username"]),
                Subject = "Nova notifikacija",
                Body = request.Content,
                IsBodyHtml = false
            };

            mail.To.Add(request.Email);

            try
            {
                await smtpClient.SendMailAsync(mail);
                return Ok("Email sent");
            }
            catch
            {
                return StatusCode(500, "Greška prilikom slanja emaila");
            }
        }

        public class MailRequest
        {
            public string Email { get; set; }
            public string Content { get; set; }
        }
    }
}