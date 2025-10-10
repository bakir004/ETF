using Microsoft.EntityFrameworkCore.Migrations;

#nullable disable

namespace ooadepazar.Migrations
{
    /// <inheritdoc />
    public partial class ikre : Migration
    {
        /// <inheritdoc />
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropForeignKey(
                name: "FK_Narudzba_AspNetUsers_KurirskaSluzbaIDId",
                table: "Narudzba");

            migrationBuilder.RenameColumn(
                name: "KurirskaSluzbaIDId",
                table: "Narudzba",
                newName: "KurirskaSluzbaIdId");

            migrationBuilder.RenameIndex(
                name: "IX_Narudzba_KurirskaSluzbaIDId",
                table: "Narudzba",
                newName: "IX_Narudzba_KurirskaSluzbaIdId");

            migrationBuilder.AddForeignKey(
                name: "FK_Narudzba_AspNetUsers_KurirskaSluzbaIdId",
                table: "Narudzba",
                column: "KurirskaSluzbaIdId",
                principalTable: "AspNetUsers",
                principalColumn: "Id");
        }

        /// <inheritdoc />
        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropForeignKey(
                name: "FK_Narudzba_AspNetUsers_KurirskaSluzbaIdId",
                table: "Narudzba");

            migrationBuilder.RenameColumn(
                name: "KurirskaSluzbaIdId",
                table: "Narudzba",
                newName: "KurirskaSluzbaIDId");

            migrationBuilder.RenameIndex(
                name: "IX_Narudzba_KurirskaSluzbaIdId",
                table: "Narudzba",
                newName: "IX_Narudzba_KurirskaSluzbaIDId");

            migrationBuilder.AddForeignKey(
                name: "FK_Narudzba_AspNetUsers_KurirskaSluzbaIDId",
                table: "Narudzba",
                column: "KurirskaSluzbaIDId",
                principalTable: "AspNetUsers",
                principalColumn: "Id");
        }
    }
}
